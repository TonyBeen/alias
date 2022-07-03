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

#define LOCAL_SOCK_PATH "/tmp/log_sock_server"
#define EPOLL_SIZE (32)

int gSocket = -1;
static char gRecvBuf[1024 * 8];
struct UdpClient {
    UdpClient()
    {
        memset(this, 0, sizeof(UdpClient));
    }

    UdpClient(const sockaddr_in *addr)
    {
        memmove(&client, addr, sizeof(client));
    }

    UdpClient(const UdpClient &cli)
    {
        memmove(&client, &cli.client, sizeof(client));
    }

    UdpClient &operator=(const UdpClient &cli)
    {
        memmove(&client, &cli.client, sizeof(client));
    }

    sockaddr_in client;
};

std::map<uint32_t, UdpClient> gNetClientMap;

void print(const char *perfix)
{
    printf("%s\n", perfix);
    printf("-h get help\n");
    printf("-s /path/ save log file to /path.(default not to save)\n");
    printf("-p port listen on port.(default is 8000)\n");
    exit(0);
}

void signal_interrupt(int sig)
{
    printf(" SIGINT capture sig: %d\n", sig);
    unlink(LOCAL_SOCK_PATH);
    exit(0);
}
void signal_quit(int sig)
{
    printf(" SIGQUIT capture sig: %d\n", sig);
    unlink(LOCAL_SOCK_PATH);
    exit(0);
}

int InitSocket()
{
    gSocket = ::socket(AF_LOCAL, SOCK_STREAM, 0);
    if (gSocket < 0) {
        printf("%s() socket error. %d %s\n", __func__, errno, strerror(errno));
        return gSocket;
    }

    unlink(LOCAL_SOCK_PATH);
    sockaddr_un saddr;
    saddr.sun_family = AF_LOCAL;
    snprintf(saddr.sun_path, sizeof(saddr.sun_path), LOCAL_SOCK_PATH);
    int nRetCode = ::bind(gSocket, (sockaddr *)&saddr, sizeof(saddr));
    if (nRetCode < 0) {
        printf("%s() bind error. %d %s\n", __func__, errno, strerror(errno));
        goto error;
    }

    nRetCode = ::listen(gSocket, 14);
    if (nRetCode < 0) {
        printf("%s() listen error. %d %s\n", __func__, errno, strerror(errno));
        goto error;
    }

    return gSocket;

error:
    if (gSocket > 0) {
        ::close(gSocket);
        gSocket = -1;
    }

    return nRetCode;
}

void OnRecvBuf(int sock, int size)
{
    if (size <= 0) {
        return;
    }
    static socklen_t len = sizeof(sockaddr_in);
    for (const auto &it : gNetClientMap) {
        ::sendto(sock, gRecvBuf, size, 0, (sockaddr *)&it.second.client, len);
    }
}


int main(int argc, char **argv)
{
    signal(SIGINT, signal_interrupt);
    signal(SIGQUIT, signal_quit);
    signal(SIGPIPE, SIG_IGN);

    char c = '\0';
    int port = 0;
    bool isSaveFile = false;
    std::string path;
    while ((c = ::getopt(argc, argv, "hs:p:")) != -1) {
        switch (c) {
        case 'h':
            print(argv[0]);
            break;
        case 's':
            isSaveFile = true;
            path = optarg;
            break;
        case 'p':
            port = atoi(optarg);
        default:
            break;
        }
    }

    assert(InitSocket() > 0);
    
    int netSocket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (netSocket < 0) {
        printf("%s() socket error. %d %s\n", __func__, errno, strerror(errno));
        return -1;
    }

    sockaddr_in srvAddr;
    srvAddr.sin_family = AF_INET;
    srvAddr.sin_port = htons(port > 0 ? port : 8000);
    srvAddr.sin_addr.s_addr = INADDR_ANY;
    int retCode = ::bind(netSocket, (sockaddr *)&srvAddr, sizeof(srvAddr));
    if (retCode < 0) {
        printf("%s() bind error. %d %s\n", __func__, errno, strerror(errno));
        return -1;
    }

    epoll_event events[EPOLL_SIZE];
    int epollFd = epoll_create(EPOLL_SIZE);
    if (epollFd < 0) {
        printf("%s() epoll_create error. %d %s\n", __func__, errno, strerror(errno));
        return -1;
    }

    int flag = fcntl(netSocket, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(netSocket, F_SETFL, flag);

    epoll_event event;
    event.data.fd = gSocket;
    event.events = EPOLLET | EPOLLIN;
    assert(epoll_ctl(epollFd, EPOLL_CTL_ADD, gSocket, &event) == 0);
    event.data.fd = netSocket;
    event.events = EPOLLET | EPOLLIN;
    assert(epoll_ctl(epollFd, EPOLL_CTL_ADD, netSocket, &event) == 0);

    printf("gSocket = %d, netSocket = %d, epollFd = %d, waiting...\n", gSocket, netSocket, epollFd);
    while (true) {
        int nev = epoll_wait(epollFd, events, EPOLL_SIZE, -1);
        if (nev < 0) {
            printf("epoll_wait error. [%d,%s]", errno, strerror(errno));
            exit(0);
        }
        for (int i = 0; i < nev; ++i) {
            epoll_event &ev = events[i];
            if (ev.data.fd == gSocket) {
                sockaddr_un client;
                socklen_t len = sizeof(client);
                int cfd = ::accept(gSocket, (sockaddr *)&client, &len);
                if (cfd < 0) {
                    perror("accept error");
                } else if (cfd > 0) {
                    printf("accept client. %d %s\n", cfd, client.sun_path);
                    flag = fcntl(cfd, F_GETFL);
                    flag |= O_NONBLOCK;
                    fcntl(cfd, F_SETFL, flag);

                    event.data.fd = cfd;
                    event.events = EPOLLET | EPOLLIN;
                    epoll_ctl(epollFd, EPOLL_CTL_ADD, cfd, &event);
                }
                continue;
            }

            if (ev.data.fd == netSocket) {
                static uint32_t clientCount = 0;
                sockaddr_in client;
                socklen_t len = sizeof(client);
                static char buf[64];
                memset(buf, 0, sizeof(buf));
                int size = ::recvfrom(netSocket, buf, sizeof(buf), 0, (sockaddr *)&client, &len);
                if (size < 0) {
                    perror("recvfrom error");
                    continue;
                }
                if (size > 0 && strcasecmp("connect", buf) == 0) {      // udp accept
                    gNetClientMap[++clientCount] = UdpClient(&client);
                    int n = sprintf(buf, "client id = %d\r\n", clientCount);
                    ::sendto(netSocket, buf, n, 0, (sockaddr *)&client, len);
                }
                if (size > 0) {     // udp close
                    char *index = strstr(buf, "close");
                    if (index) {
                        int clientID = atoi(index + 5);
                        printf("client Id %d quit. [%s:%d]\n",
                            clientID, inet_ntoa(client.sin_addr), ntohs(client.sin_port));
                        gNetClientMap.erase(clientID);
                    }
                }
            }

            if (ev.events & EPOLLIN) {  // 读事件
                memset(gRecvBuf, 0, sizeof(gRecvBuf));
                int ret = recv(ev.data.fd, gRecvBuf, sizeof(gRecvBuf), 0);
                if (ret < 0) {
                    printf("%d, %d, %s\n", ev.data.fd, errno, strerror(errno));
                    if (errno != EAGAIN) {
                        exit(0);
                    }
                }
                if (ret > 0) {
                    printf("%s", gRecvBuf);
                    OnRecvBuf(netSocket, ret);
                }
            }

            if (ev.events & EPOLLHUP) { // 退出事件
                printf("EPOLLHUP client %d exit.\n", ev.data.fd);
                close(ev.data.fd);
                epoll_ctl(epollFd, EPOLL_CTL_DEL, ev.data.fd, nullptr);
            }
        }
    }

    return 0;
}
