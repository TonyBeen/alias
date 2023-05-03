/*************************************************************************
    > File Name: logcat.cc
    > Author: hsz
    > Brief: server of log console
    > Created Time: Sun 23 Jan 2022 04:36:06 PM CST
 ************************************************************************/

#include <iostream>
#include <string>
#include <error.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <map>
#include <json/json.h>
#include "callstack.h"
#include "log.h"

#define LOCAL_SOCK_PATH "/tmp/log_sock_server"
#define EPOLL_SIZE      (1024)

#define LOG_TAG "logcat"

int gLocalServerSocket = -1;    // 本地套接字服务端
int gLocalClientSocket = -1;    // 本地套接字客户端
int gNetServerSocket = -1;      // 网络套接字服务端
std::map<int, sockaddr_in> gNetClientMap; // 网络套接字客户端map

Json::String gJsonNotice;

static char gRecvBuf[1024 * 8];

void print(const char *perfix)
{
    printf("%s\n", perfix);
    printf("-h get help\n");
    printf("-p port listen on port.(default is 8000)\n");
    exit(0);
}

void catch_signal(int sig)
{
    std::string signalMsg;
    std::string strCallStack;
    eular::CallStack stack;
    char msgBuf[256] = {0};
    Json::Value root;

    switch (sig) {
        case SIGINT:
            snprintf(msgBuf, sizeof(msgBuf), "SIGINT captured sig: %d\n", sig);
            break;
        case SIGABRT:
            snprintf(msgBuf, sizeof(msgBuf), "SIGABRT captured sig: %d\n", sig);
            stack.update();
            strCallStack = stack.toString();
            break;
        case SIGQUIT:
            snprintf(msgBuf, sizeof(msgBuf), "SIGQUIT captured sig: %d\n", sig);
            break;
        case SIGSEGV:
            snprintf(msgBuf, sizeof(msgBuf), "SIGSEGV captured sig: %d\n", sig);
            stack.update();
            strCallStack = stack.toString();
            break;
        default:
            printf("unhandle signal %d\n", sig);
            return;
    }

    signalMsg = msgBuf;
    signalMsg.append(strCallStack);

    root["id"] = "error";
    root["keywords"] = "msg";
    root["msg"] = signalMsg;

    const std::string &jsonMsg = Json::FastWriter().write(root);

    for (auto it = gNetClientMap.begin(); it != gNetClientMap.end();) {
        ::send(it->first, jsonMsg.c_str(), jsonMsg.length(), 0);
        close(it->first);
        it = gNetClientMap.erase(it);
    }

    close(gNetServerSocket);
    close(gLocalClientSocket);
    close(gLocalServerSocket);
    unlink(LOCAL_SOCK_PATH);
    exit(0);
}

int InitSocket()
{
    gLocalServerSocket = ::socket(AF_LOCAL, SOCK_STREAM, 0);
    if (gLocalServerSocket < 0) {
        printf("%s() socket error. %d %s\n", __func__, errno, strerror(errno));
        return gLocalServerSocket;
    }

    unlink(LOCAL_SOCK_PATH);
    sockaddr_un saddr;
    saddr.sun_family = AF_LOCAL;
    snprintf(saddr.sun_path, sizeof(saddr.sun_path), LOCAL_SOCK_PATH);
    int nRetCode = ::bind(gLocalServerSocket, (sockaddr *)&saddr, sizeof(saddr));
    if (nRetCode < 0) {
        printf("%s() bind error. %d %s\n", __func__, errno, strerror(errno));
        goto error;
    }

    nRetCode = ::listen(gLocalServerSocket, 14);
    if (nRetCode < 0) {
        printf("%s() listen error. %d %s\n", __func__, errno, strerror(errno));
        goto error;
    }

    return gLocalServerSocket;

error:
    if (gLocalServerSocket > 0) {
        ::close(gLocalServerSocket);
        gLocalServerSocket = -1;
    }

    return nRetCode;
}

void OnLocalSocketReadEvent(const std::string &jsonContent)
{
    if (jsonContent.length() == 0) {
        return;
    }

    printf("json: \n%s\n", jsonContent.c_str());

    Json::Reader jsonReader;
    Json::Value root;
    if (jsonReader.parse(jsonContent, root) == false) {
        return;
    }

    auto &jsonId = root["id"];
    if (jsonId.asString() == "notice") {
        gJsonNotice = jsonContent;
    }

    for (auto it  = gNetClientMap.begin(); it != gNetClientMap.end(); ++it) {
        ::send(it->first, jsonContent.c_str(), jsonContent.length(), 0);
    }
}

void OnTcpSocketReadEvent(const std::string &jsonContent)
{
    (void)jsonContent;
    // TODO 解析json
}

int main(int argc, char **argv)
{
    char c = '\0';
    int port = 0;
    std::string path;
    while ((c = ::getopt(argc, argv, "hs:p:")) != -1) {
        switch (c) {
        case 'h':
            print(argv[0]);
            break;
        case 'p':
            port = atoi(optarg);
        default:
            break;
        }
    }

    signal(SIGINT, catch_signal);
    signal(SIGQUIT, catch_signal);
    signal(SIGABRT, catch_signal);
    signal(SIGSEGV, catch_signal);
    signal(SIGPIPE, SIG_IGN);

    assert(InitSocket() > 0);

    int gNetServerSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (gNetServerSocket < 0) {
        printf("%s() socket error. %d %s\n", __func__, errno, strerror(errno));
        return -1;
    }

    sockaddr_in srvAddr;
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(port > 0 ? port : 8000);
    srvAddr.sin_addr.s_addr = INADDR_ANY;
    int retCode = ::bind(gNetServerSocket, (sockaddr *)&srvAddr, sizeof(srvAddr));
    if (retCode < 0) {
        printf("%s() bind error. %d %s\n", __func__, errno, strerror(errno));
        return -1;
    }

    ::listen(gNetServerSocket, 128);

    epoll_event events[EPOLL_SIZE];
    int epollFd = epoll_create(EPOLL_SIZE);
    if (epollFd < 0) {
        printf("%s() epoll_create error. %d %s\n", __func__, errno, strerror(errno));
        return -1;
    }

    int flag = fcntl(gNetServerSocket, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(gNetServerSocket, F_SETFL, flag);

    epoll_event event;
    event.data.fd = gLocalServerSocket;
    event.events = EPOLLIN;
    assert(epoll_ctl(epollFd, EPOLL_CTL_ADD, gLocalServerSocket, &event) == 0);

    event.data.fd = gNetServerSocket;
    event.events = EPOLLIN;
    assert(epoll_ctl(epollFd, EPOLL_CTL_ADD, gNetServerSocket, &event) == 0);

    printf("LocalServerSocket = %d, NetServerSocket = %d, epollFd = %d, waiting...\n", gLocalServerSocket, gNetServerSocket, epollFd);
    while (true) {
        int nev = epoll_wait(epollFd, events, EPOLL_SIZE, -1);
        if (nev < 0) {
            printf("epoll_wait error. [%d,%s]\n", errno, strerror(errno));
            exit(0);
        }
        for (int i = 0; i < nev; ++i) {
            epoll_event &ev = events[i];
            if (ev.data.fd == gLocalServerSocket && gLocalClientSocket <= 0) {
                sockaddr_un client;
                socklen_t len = sizeof(client);
                gLocalClientSocket = ::accept(gLocalServerSocket, (sockaddr *)&client, &len);
                if (gLocalClientSocket <= 0) {
                    perror("accept error");
                } else {
                    printf("accept client. %d %s\n", gLocalClientSocket, client.sun_path);
                    flag = fcntl(gLocalClientSocket, F_GETFL);
                    flag |= O_NONBLOCK;
                    fcntl(gLocalClientSocket, F_SETFL, flag);

                    event.data.fd = gLocalClientSocket;
                    event.events = EPOLLIN;
                    epoll_ctl(epollFd, EPOLL_CTL_ADD, gLocalClientSocket, &event);
                }
                continue;
            }

            if (ev.data.fd == gNetServerSocket) {
                sockaddr_in clientAddr;
                socklen_t addrLen = sizeof(sockaddr_in);

                int clientFd = ::accept(gNetServerSocket, (sockaddr *)&clientAddr, &addrLen);
                if (clientFd > 0) {
                    flag = fcntl(clientFd, F_GETFL);
                    flag |= O_NONBLOCK;
                    fcntl(clientFd, F_SETFL, flag);

                    epoll_event epEvent;
                    epEvent.data.fd = clientFd;
                    epEvent.events = EPOLLIN;
                    int32_t nRet = epoll_ctl(epollFd, EPOLL_CTL_ADD, gLocalServerSocket, &epEvent);
                    if (0 != nRet) {
                        static const char *errorMsgJson = "{\"id\": \"error\", \"keywords\": [\"msg\"], \"msg\": \"%s\"}";
                        char msg[256] = { '\0' };
                        int32_t nFormat = snprintf(msg, sizeof(msg), errorMsgJson, strerror(errno));
                        if (nFormat > 0) {
                            ::send(clientFd, msg, nFormat, 0);
                        } else {
                            perror("epoll_ctl error");
                        }
                        close(clientFd);
                    } else {
                        gNetClientMap[clientFd] = clientAddr;
                        ::send(clientFd, gJsonNotice.c_str(), gJsonNotice.length(), 0);
                    }
                }
            }

            if (ev.events & EPOLLIN) {  // 本地套接字读事件
                std::string jsonContent;
                static const char *strSeparator = "\r\n\r\n";
                static const uint32_t nSeparatorLen = strlen(strSeparator);

                while (true) {
                    memset(gRecvBuf, 0, sizeof(gRecvBuf));
                    int32_t nRecv = ::recv(ev.data.fd, gRecvBuf, sizeof(gRecvBuf), MSG_PEEK);
                    if (nRecv == 0) { // 没有携带结束符
                        printf("Invalid data segment\n");
                        jsonContent.clear();
                        break;
                    }
                    if (nRecv < 0) {
                        printf("%d, %d, %s\n", ev.data.fd, errno, strerror(errno));
                        if (errno != EAGAIN) {
                            close(ev.data.fd);
                            epoll_ctl(epollFd, EPOLL_CTL_DEL, ev.data.fd, nullptr);
                        }
                        break;
                    }
                    void *pFound = memmem(gRecvBuf, nRecv, strSeparator, nSeparatorLen);
                    if (pFound) {
                        nRecv = ((char *)pFound - gRecvBuf);
                        jsonContent.append(gRecvBuf, nRecv);
                        ::recv(ev.data.fd, gRecvBuf, nRecv, 0);
                        ::recv(ev.data.fd, gRecvBuf, nSeparatorLen, 0); // 从缓存中移除分割符
                        break;
                    } else {
                        jsonContent.append(gRecvBuf, nRecv);
                        ::recv(ev.data.fd, gRecvBuf, nRecv, 0);
                    }
                }

                if (ev.data.fd == gLocalClientSocket) { // 将本地套接字发送的数据转发
                    OnLocalSocketReadEvent(jsonContent);
                } else {
                    OnTcpSocketReadEvent(jsonContent);
                }
            }

            if (ev.events & (EPOLLHUP | EPOLLRDHUP | EPOLLERR)) { // 退出事件
                printf("EPOLLERR 0x%x client %d exit.\n", ev.events, ev.data.fd);
                close(ev.data.fd);
                epoll_ctl(epollFd, EPOLL_CTL_DEL, ev.data.fd, nullptr);
                if (ev.data.fd == gLocalClientSocket) {
                    gLocalClientSocket = -1;
                } else {
                    gNetClientMap.erase(ev.data.fd);
                }
            }
        }
    }

    return 0;
}
