#include "log_write.h"
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/mman.h>

namespace Jarvis {
LogWrite::LogWrite()
{
    int ret = 0;
    pthread_mutexattr_t mutexAttr;
    pthread_mutexattr_init(&mutexAttr);
#if defined(_POSIX_THREAD_PROCESS_SHARED)
    pthread_mutexattr_setpshared(&mutexAttr, PTHREAD_PROCESS_SHARED);
    mMutex = (pthread_mutex_t *)mmap(nullptr, sizeof(pthread_mutex_t),
        PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
#else
    mMutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
#endif
    assert(mMutex);
    do {
        ret = pthread_mutex_init(mMutex, &mutexAttr);
    } while (ret == EAGAIN);
    assert(ret == 0);
    pthread_mutexattr_destroy(&mutexAttr);
}

LogWrite::~LogWrite()
{
    if (!mMutex) {
        return;
    }
    pthread_mutex_destroy(mMutex);
#if defined(_POSIX_THREAD_PROCESS_SHARED)
    munmap(mMutex, sizeof(pthread_mutex_t));
#else
    free(mMutex);
#endif
    mMutex = nullptr;
}

ssize_t StdoutLogWrite::WriteToFile(std::string msg)
{
    if (msg.length() == 0) {
        return 0;
    }
    ssize_t ret = 0;
    pthread_mutex_lock(mMutex);
    ret = write(STDOUT_FILENO, msg.c_str(), msg.length());
    pthread_mutex_unlock(mMutex);
    if (ret > 0) {
        return ret;
    }
    return 0;
}

std::string StdoutLogWrite::getFileName()
{
    return std::string("stdout");
}

size_t StdoutLogWrite::getFileSize()
{
    return 0;
}

uint32_t StdoutLogWrite::getFileMode()
{
    return 0;
}

bool StdoutLogWrite::setFileMode(uint32_t mode)
{
    return true;
}

uint32_t StdoutLogWrite::getFileFlag()
{
    return 0;
}

bool StdoutLogWrite::setFileFlag(uint32_t flag)
{
    return true;
}

bool StdoutLogWrite::CreateNewFile(std::string fileName)
{
    return true;
}

bool StdoutLogWrite::CloseFile()
{
    return true;
}

FileLogWrite::FileLogWrite(uint32_t fileFlag, uint32_t fileMode) :
    mFileFlag(fileFlag),
    mFileMode(fileMode)
{
    mFileDesc = (int *)mmap(nullptr, sizeof(int), 
        PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    mFileSize = (uint64_t *)mmap(nullptr, sizeof(uint64_t), 
        PROT_WRITE|PROT_READ, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    assert(mFileDesc && mFileSize);
    pthread_mutex_lock(mMutex);
    std::string fileName = getFileName();
    CreateNewFile(fileName);
    pthread_mutex_unlock(mMutex);
}

FileLogWrite::~FileLogWrite()
{
    CloseFile();
    munmap(mFileDesc, sizeof(int));
    munmap(mFileSize, sizeof(uint64_t));
}

std::string FileLogWrite::getFileName()
{
    time_t curr = time(nullptr);
    struct tm *p = localtime(&curr);

    char buf[1024] = {0};
    sprintf(buf, "log-%.4d%.2d%.2d-%.2d%.2d%.2d.log",
        1900 + p->tm_year,
        1 + p->tm_mon,
        p->tm_mday,
        p->tm_hour,
        p->tm_min,
        p->tm_sec);
    return std::string(buf);
}

ssize_t FileLogWrite::WriteToFile(std::string msg)
{
    pthread_mutex_lock(mMutex);
    ssize_t ret = 0;
    if (*mFileDesc <= 0 || *mFileSize > MAX_FILE_SIZE) {
        CloseFile();
        CreateNewFile(getFileName());
    }
    if (msg.length()) {
        ret = write(*mFileDesc, msg.c_str(), msg.length());
    }
    if (ret >= 0) {
        *mFileSize += ret;
    }
    pthread_mutex_unlock(mMutex);
    return ret;
}
size_t FileLogWrite::getFileSize()
{
    return *mFileSize;
}

uint32_t FileLogWrite::getFileMode()
{
    return mFileMode;
}
bool FileLogWrite::setFileMode(uint32_t mode)
{
    mFileMode = mode;
}
uint32_t FileLogWrite::getFileFlag()
{
    return mFileFlag;
}
bool FileLogWrite::setFileFlag(uint32_t flag)
{
    mFileFlag = flag;
}

static  bool __lstat(const char *path)
{
    struct stat lst;
    int ret = lstat(path, &lst);
    return ret;
}

static  bool __mkdir(const char *path)
{
    if(access(path, F_OK) == 0) {
        return 0;
    }
    return mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

bool Mkdir(const std::string &path)
{
    if(__lstat(path.c_str()) == 0) {
        return true;
    }
    char* filePath = strdup(path.c_str());
    char* ptr = strchr(filePath + 1, '/');
    do {
        for(; ptr; *ptr = '/', ptr = strchr(ptr + 1, '/')) {
            *ptr = '\0';
            if(__mkdir(filePath) != 0) {
                break;
            }
        }
        if(ptr != nullptr) {
            break;
        } else if(__mkdir(filePath) != 0) {
            break;
        }
        free(filePath);
        return true;
    } while(0);
    free(filePath);
    return false;
}

bool FileLogWrite::CreateNewFile(std::string fileName)
{
    if (*mFileDesc > 0 && *mFileSize < MAX_FILE_SIZE) {
        return true;
    }
    std::string path = "/home/hsz/log/";
    bool ret = Mkdir(path);
    path += fileName;

    *mFileDesc = open(path.c_str(), mFileFlag, mFileMode);
    if (*mFileDesc < 0) {
        perror("open file error");
        return false;
    }
    *mFileSize = 0;
    return true;
}

bool FileLogWrite::CloseFile()
{
    if (*mFileDesc <= 0) {
        return true;
    }
    return !close(*mFileDesc);
}

#define LOCAL_SOCKET_SERVER_PATH        "/tmp/log_sock_server"
#define LOCAL_SOCKET_CLIENT_PATH_FMT    "/tmp/log_sock_client_%d"   // %d 是父进程的ID，使用子进程ID时多进程时容易出问题
#define MAX_SIZE_OF_SUNPATH             108
static std::string gClientSockPath;

ConsoleLogWrite::ConsoleLogWrite() :
    mClientFd(-1)
{
    mClientSockAddr.sun_family = AF_LOCAL;
    snprintf(mClientSockAddr.sun_path, MAX_SIZE_OF_SUNPATH, LOCAL_SOCKET_CLIENT_PATH_FMT, getppid());
    mLocalClientSockPath = mClientSockAddr.sun_path;
    gClientSockPath = mLocalClientSockPath;

    mServerSockAddr.sun_family = AF_LOCAL;
    snprintf(mServerSockAddr.sun_path, MAX_SIZE_OF_SUNPATH, LOCAL_SOCKET_SERVER_PATH);
    mLocalServerSockPath = mServerSockAddr.sun_path;
    
    // 捕获信号
    signal(SIGINT, signalHandler);
    signal(SIGQUIT, signalHandler);
    signal(SIGPIPE, signalHandler);

    InitParams();
}

ConsoleLogWrite::~ConsoleLogWrite()
{
    Destroy();
}

void ConsoleLogWrite::InitParams()
{
    Destroy();
    mClientFd = ::socket(AF_LOCAL, SOCK_STREAM, 0);
    if (mClientFd < 0) {
        perror("socket error:");
        return;
    }
    do {
        int nRetCode = ::bind(mClientFd, (const sockaddr *)&mClientSockAddr, sizeof(mClientSockAddr));
        if (nRetCode < 0) {
            break;
        }
        nRetCode = ::connect(mClientFd, (sockaddr *)&mServerSockAddr, sizeof(mServerSockAddr));
        if (nRetCode < 0) {
            break;
        }
        return;
    } while(0);
    close(mClientFd);
    mClientFd = -1;
}

void ConsoleLogWrite::Destroy()
{
    if (mClientFd > 0) {
        close(mClientFd);
        mClientFd = -1;
    }
    unlink(mLocalClientSockPath.c_str());
}

void ConsoleLogWrite::signalHandler(int sig)
{
    if (sig == SIGPIPE) {
        return;
    }
    unlink(gClientSockPath.c_str());
    exit(0);
}

ssize_t ConsoleLogWrite::WriteToFile(std::string msg)
{
    if (mClientFd <= 0) {
        InitParams();
        if (mClientFd <=0 ) {
            return 0;
        }
    }
    pthread_mutex_lock(mMutex);
    ssize_t sendSize = ::send(mClientFd, msg.c_str(), msg.length(), 0);
    if (sendSize < 0) { // 服务端不在线
        Destroy();
    }
    pthread_mutex_unlock(mMutex);
    return sendSize;
}

std::string ConsoleLogWrite::getFileName()
{
    return mLocalClientSockPath;
}

size_t ConsoleLogWrite::getFileSize()
{
    return 0;
}

uint32_t ConsoleLogWrite::getFileMode()
{
    return 0;
}

bool ConsoleLogWrite::setFileMode(uint32_t mode)
{
    return true;
}

uint32_t ConsoleLogWrite::getFileFlag()
{
    return 0;
}

bool ConsoleLogWrite::setFileFlag(uint32_t flag)
{
    return true;
}

bool ConsoleLogWrite::CreateNewFile(std::string fileName)
{
    return true;
}

bool ConsoleLogWrite::CloseFile()
{
    return true;
}

}
