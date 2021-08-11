/*************************************************************************
    > File Name: log_write.h
    > Author: hsz
    > Desc: write log to file
    > Created Time: 2021年04月12日 星期一 22时16分35秒
 ************************************************************************/

#ifndef __LOG_WRITE_H__
#define __LOG_WRITE_H__

#include <pthread.h>

#include <string>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAX_FILE_SIZE (1024 * 1024 * 5)  // one file' max size is 5mb

namespace Alias {
class LogWrite {
public:
    LogWrite();
    LogWrite(const LogWrite&) = delete;
    LogWrite& operator= (const LogWrite&) = delete;
    virtual ~LogWrite(){}

    virtual size_t       WriteToFile(std::string msg) = 0;
    virtual std::string  getFileName() = 0;
    virtual size_t       getFileSize() = 0;
    virtual uint32_t     getFileMode() = 0;
    virtual bool         setFileMode(uint32_t mode) = 0;
    virtual uint32_t     getFileFlag() = 0;
    virtual bool         setFileFlag(uint32_t flag) = 0;

    virtual bool         CreateNewFile(std::string fileName) = 0;
    virtual bool         CloseFile(const int fd) = 0;
protected:
    pthread_mutex_t mMutex;        // 同步状态下保护文件描述符
};

class StdoutLogWrite : public LogWrite {
public:
    StdoutLogWrite() {}
    ~StdoutLogWrite() {}

    virtual size_t       WriteToFile(std::string msg);
    virtual std::string  getFileName();
    virtual size_t       getFileSize();
    virtual uint32_t     getFileMode();
    virtual bool         setFileMode(uint32_t mode);
    virtual uint32_t     getFileFlag();
    virtual bool         setFileFlag(uint32_t flag);

    virtual bool         CreateNewFile(std::string fileName);
    virtual bool         CloseFile(const int fd);
private:
    bool isInterrupt;
};

class FileLogWrite : public LogWrite {
public:
    FileLogWrite(uint32_t fileFlag = O_WRONLY | O_CREAT | O_APPEND, uint32_t fileMode = 0664);
    virtual ~FileLogWrite();

    virtual size_t       WriteToFile(std::string msg);
    virtual std::string  getFileName();
    virtual size_t       getFileSize();
    virtual uint32_t     getFileMode();
    virtual bool         setFileMode(uint32_t mode);
    virtual uint32_t     getFileFlag();
    virtual bool         setFileFlag(uint32_t flag);

    virtual bool         CreateNewFile(std::string fileName);
    virtual bool         CloseFile(const int fd);

private:
    bool isInterrupt;
    int mFileDesc;
    uint32_t mFileMode;
    uint32_t mFileFlag;
    std::string mFileName;
    size_t mFileSize;
};


} // namespace Alias
#endif // __LOG_WRITE_H__
