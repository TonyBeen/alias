#include "log_write.h"
#include <assert.h>
#include <time.h>
#include <sys/time.h>

namespace Alias {
LogWrite::LogWrite()
{
    int ret = 0;
    do {
        ret = pthread_mutex_init(&mMutex, nullptr);
    } while (ret == EAGAIN);
    assert(ret == 0);
}

size_t StdoutLogWrite::WriteToFile(std::string msg)
{
    if (msg.length() == 0) {
        return 0;
    }
    ssize_t ret = 0;
    pthread_mutex_lock(&mMutex);
    ret = write(STDOUT_FILENO, msg.c_str(), msg.length());
    pthread_mutex_unlock(&mMutex);
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

bool StdoutLogWrite::CloseFile(const int fd)
{
    return true;
}

FileLogWrite::FileLogWrite(uint32_t fileFlag, uint32_t fileMode) :
    mFileFlag(fileFlag),
    mFileMode(fileMode)
{
    std::string fileName = getFileName();
    CreateNewFile(fileName);
}
FileLogWrite::~FileLogWrite()
{
    CloseFile(mFileDesc);
}

std::string FileLogWrite::getFileName()
{
    time_t curr = time(nullptr);
    struct tm *p = localtime(&curr);

    char buf[1024] = {0};
    sprintf(buf, "alias log %.4d%.2d%.2d-%.2d%.2d%.2d.log",
        1900 + p->tm_year,
        1 + p->tm_mon,
        p->tm_mday,
        p->tm_hour,
        p->tm_min,
        p->tm_sec);
    return std::string(buf);
}

size_t FileLogWrite::WriteToFile(std::string msg)
{
    pthread_mutex_lock(&mMutex);
    int ret = 0;
    if (mFileDesc <= 0 || mFileSize > MAX_FILE_SIZE) {
        CloseFile(mFileDesc);
        CreateNewFile(getFileName());
    }
    if (msg.length()) {
        ret = write(mFileDesc, msg.c_str(), msg.length());
    }
    if (ret >= 0) {
        mFileSize += ret;
    } else if (ret < 0) {
        perror("write error");
    }
    pthread_mutex_unlock(&mMutex);
    return ret;
}
size_t   FileLogWrite::getFileSize()
{
    return mFileSize;
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

bool FileLogWrite::CreateNewFile(std::string fileName)
{
    if (mFileDesc > 0 && mFileSize < MAX_FILE_SIZE) {
        return true;
    }
    std::string path = "~/log/";
    bool ret = Mkdir(path);
    path += fileName;

    mFileDesc = open(path.c_str(), mFileFlag, mFileMode);
    if (mFileDesc < 0) {
        perror("open file error");
        return false;
    }
    mFileSize = 0;
    return true;
}

bool FileLogWrite::CloseFile(const int fd)
{
    if (fd <= 0) {
        return true;
    }
    return !close(fd);
}

}
