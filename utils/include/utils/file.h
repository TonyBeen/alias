/*************************************************************************
    > File Name: file.h
    > Author: hsz
    > Brief:
    > Created Time: Wed 14 Sep 2022 04:56:00 PM CST
 ************************************************************************/

#ifndef __EULAR_FILE_H__
#define __EULAR_FILE_H__

#include <stdint.h>
#include <memory>

#include <sys/stat.h>

#include <utils/sysdef.h>
#include <utils/string8.h>
#include <utils/buffer.h>

#if defined(OS_WINDOWS)
#if !defined(_UNICODE) || !defined(UNICODE)
#error "The project needs unicode configuration"
#endif

#define file_stat_t    struct _stat 

#else
#define file_stat_t    struct stat
#endif

namespace eular {

class FileInfo final
{
    friend class File;

public:
    typedef std::unique_ptr<FileInfo>   Ptr;
    typedef std::shared_ptr<FileInfo>   SP;
    typedef std::weak_ptr<FileInfo>     WP;

    FileInfo();
    FileInfo(const String8 &path);
    ~FileInfo();

    FileInfo(const FileInfo &other);
    FileInfo &operator=(const FileInfo &other);

    bool    execGetInfo(const String8 &path);
    int64_t getFileSize() const;
    time_t  getModifyTime() const;
    time_t  getCreateTime() const;
    int32_t getFileUid() const;
    const file_stat_t &getFileStat() const { return mFileInfo; }

    static bool     FileExist(const String8 &path);
    static bool     GetFileStat(const String8 &path, file_stat_t *fileStat);
    static int64_t  GetFileSize(const String8 &path);
    static time_t   GetFileCreateTime(const String8 &path);
    static time_t   GetFileModifyTime(const String8 &path);
    static int32_t  GetFileUid(const String8 &path);

private:
    String8     mFilePath;
    file_stat_t mFileInfo;
};

class File final
{
    File(const File &other) = delete;
    File &operator=(const File &other) = delete;

public:
    typedef std::unique_ptr<File>   Ptr;
    typedef std::shared_ptr<File>   SP;
    typedef std::weak_ptr<File>     WP;

    enum OpenModeFlag {
        NotOpen         = 0x0000,   // 不打开文件, 只获取文件信息
        ReadOnly        = 0x0001,   // 只读
        WriteOnly       = 0x0002,   // 只写
        ReadWrite       = 0x0004,   // 读写, 禁止ReadOnly与WriteOnly相或
        Create          = 0x0008,   // 创建文件, 不影响已存在的文件
        Append          = 0x0010,   // 在文件末尾追加
        Truncate        = 0x0020,   // 截断文件, 当文件不为空时会清空
        Binary          = 0x0040,   // 二进制方式打开, linux下无效
        Temporary       = 0x0080,   // 创建的文件是临时文件, 在关闭文件描述符后会删除
    };

    File();
    File(const String8 &path, const String8 &mode);
    File(const FileInfo &fileInfo, const String8 &mode);
    ~File();

    bool open(const String8 &path, const String8 &mode);
    void close();
    bool opened() const;
    bool remove();
    bool rename(const String8 &name);
    void clear();
    bool eof() const;
    bool seek(size_t offset, int whence = SEEK_SET);
    bool flush();
    int64_t tell() const;
    int64_t size() const;
    int64_t read(void *buf, size_t size);
    int64_t readall(ByteBuffer &buf);
    int64_t readline(ByteBuffer &buf);
    int64_t readline(String8 &buf);
    int64_t write(const void *buf, size_t size);
    int64_t write(const ByteBuffer &buf);
    int64_t write(const String8 &buf);

private:
    FILE *      mFileDesc;
    String8     mOpenMode;
    FileInfo    mFileInfo;
};

} // namespace eular

#endif
