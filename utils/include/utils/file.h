/*************************************************************************
    > File Name: file.h
    > Author: hsz
    > Brief:
    > Created Time: Wed 14 Sep 2022 04:56:00 PM CST
 ************************************************************************/

#ifndef __EULAR_FILE_H__
#define __EULAR_FILE_H__

#include <stdio.h>
#include <memory>

#include <sys/stat.h>

#include <utils/string8.h>
#include <utils/buffer.h>

namespace eular {

class FileInfo final
{
    friend class File;

public:
    FileInfo();
    FileInfo(const String8 &path);
    ~FileInfo();

    FileInfo(const FileInfo &other);
    FileInfo &operator=(const FileInfo &other);

    bool    execGetInfo(const String8 &path);
    ssize_t getFileSize() const;
    time_t  getModifyTime() const;
    time_t  getCreateTime() const;
    int32_t getFileUid() const;
    const struct stat &getFileStat() const { return mFileInfo; }

    static bool     GetFileStat(const String8 &path, struct stat *fileStat);
    static int64_t  GetFileSize(const String8 &path);
    static time_t   GetFileCreateTime(const String8 &path);
    static time_t   GetFileModifyTime(const String8 &path);
    static int32_t  GetFileUid(const String8 &path);

private:
    String8     mFilePath;
    struct stat mFileInfo;
};

class File final
{
public:
    File();
    File(const String8 &path, const String8 &mode);
    File(const FileInfo &fileInfo, const String8 &mode);
    ~File();

    File(const File &other);
    File &operator=(const File &other);

    bool open(const String8 &path, const String8 &mode);
    void close();
    bool opened() const;
    bool remove();
    bool rename(const String8 &name);
    void clear();
    bool eof() const;
    bool seek(size_t offset, int whence = SEEK_SET);
    bool flush();
    ssize_t tell() const;
    ssize_t size() const;
    ssize_t read(void *buf, size_t size);
    ssize_t readall(ByteBuffer &buf);
    ssize_t readline(ByteBuffer &buf);
    ssize_t readline(String8 &buf);
    ssize_t write(const void *buf, size_t size);
    ssize_t write(const ByteBuffer &buf);
    ssize_t write(const String8 &buf);

private:
    FILE *      mFileDesc;
    String8     mOpenMode;
    FileInfo    mFileInfo;
};

} // namespace eular

#endif
