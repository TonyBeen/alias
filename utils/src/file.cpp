/*************************************************************************
    > File Name: file.cpp
    > Author: hsz
    > Brief:
    > Created Time: Wed 14 Sep 2022 04:56:15 PM CST
 ************************************************************************/

#include "utils/file.h"
#include "utils/debug.h"
#include "utils/exception.h"

namespace eular {
FileInfo::FileInfo()
{
    memset(&mFileInfo, 0, sizeof(mFileInfo));
}

FileInfo::FileInfo(const String8 &path)
{
    memset(&mFileInfo, 0, sizeof(mFileInfo));
    execGetInfo(path);
}

FileInfo::~FileInfo()
{
}

FileInfo::FileInfo(const FileInfo &other) :
    mFilePath(other.mFilePath),
    mFileInfo(other.mFileInfo)
{
}

FileInfo &FileInfo::operator=(const FileInfo &other)
{
    if (this != &other) {
        mFilePath = other.mFilePath;
        mFileInfo = other.mFileInfo;
    }

    return *this;
}

bool FileInfo::execGetInfo(const String8 &path)
{
    mFilePath = path;
    int32_t ret = stat(mFilePath.c_str(), &mFileInfo);
    if (ret) {
        char buf[128];
        sprintf(buf, "get file(%s) state error", mFilePath.c_str());
        perror(buf);
        return false;
    }

    return true;
}

int64_t FileInfo::getFileSize() const
{
    return mFileInfo.st_size;
}

time_t FileInfo::getModifyTime() const
{
    return mFileInfo.st_mtim.tv_sec;
}

time_t FileInfo::getCreateTime() const
{
    return mFileInfo.st_ctim.tv_sec;
}

int32_t FileInfo::getFileUid() const
{
    return static_cast<int32_t>(mFileInfo.st_uid);
}

bool FileInfo::FileExist(const String8 &path)
{
    if (path.empty()) {
        return false;
    }

    file_stat_t fileStat;
    int32_t ret = ::stat(path.c_str(), &fileStat);
    return ret == 0;
}

bool FileInfo::GetFileStat(const String8 &path, file_stat_t *fileStat)
{
    if (path.empty() || fileStat == nullptr) {
        return false;
    }

    int32_t ret = ::stat(path.c_str(), fileStat);
    return ret == 0;
}

int64_t FileInfo::GetFileSize(const String8 &path)
{
    struct stat fileStat;
    if (GetFileStat(path, &fileStat)) {
        return fileStat.st_size;
    }

    return -1;
}

time_t FileInfo::GetFileCreateTime(const String8 &path)
{
    struct stat fileStat;
    if (GetFileStat(path, &fileStat)) {
        return fileStat.st_ctim.tv_sec;
    }

    return -1;
}

time_t FileInfo::GetFileModifyTime(const String8 &path)
{
    struct stat fileStat;
    if (GetFileStat(path, &fileStat)) {
        return fileStat.st_mtim.tv_sec;
    }

    return -1;
}

int32_t FileInfo::GetFileUid(const String8 &path)
{
    struct stat fileStat;
    if (GetFileStat(path, &fileStat)) {
        return static_cast<int32_t>(fileStat.st_uid);
    }

    return -1;
}



File::File() :
    mFileDesc(nullptr)
{
}

File::File(const String8 &path, const String8 &mode) :
    mFileDesc(nullptr)
{
    open(path, mode);
}

File::File(const FileInfo &fileInfo, const String8 &mode) :
    mFileDesc(nullptr)
{
    mFileInfo = fileInfo;
    open(mFileInfo.mFilePath, mode);
}

File::~File()
{
    close();
}

bool File::open(const String8 &path, const String8 &mode)
{
    if (mFileDesc && mFileInfo.mFilePath == path) {
        return true;
    }

    mFileInfo.mFilePath = path;
    mOpenMode = mode;

    close();
    if (mFileInfo.execGetInfo(path)) {
        mFileDesc = fopen(path.c_str(), mode.c_str());
        if (mFileDesc == nullptr) {
            LOG("fopen error. file(%s) [%d, %s]\n", path.c_str(), errno, strerror(errno));
            return false;
        }
        
        return true;
    }
    
    return false;
}

void File::close()
{
    if (mFileDesc) {
        fclose(mFileDesc);
        mFileDesc = nullptr;
    }
}

bool File::opened() const
{
    return static_cast<bool>(mFileDesc != nullptr);
}

bool File::remove()
{
    close();
    return static_cast<bool>(0 == ::remove(mFileInfo.mFilePath.c_str()));
}

bool File::rename(const String8 &name)
{
    close();
    return static_cast<bool>(0 == ::rename(mFileInfo.mFilePath.c_str(), name.c_str()));
}

void File::clear()
{
    close();
    FILE *temp = fopen(mFileInfo.mFilePath.c_str(), "w+");
    if (temp) {
        fclose(temp);
    }

    open(mFileInfo.mFilePath.c_str(), mOpenMode);
}

bool File::eof() const
{
    if (opened()) {
        return feof(mFileDesc);
    }

    return true;
}

int64_t File::tell() const
{
    if (opened()) {
        return ftell(mFileDesc);
    }

    return -1;
}

bool File::seek(size_t offset, int32_t whence)
{
    return ::fseek(mFileDesc, offset, whence) == 0;
}

bool File::flush()
{
    return 0 == ::fflush(mFileDesc);
}

int64_t File::size() const
{
    if (opened()) {
        return mFileInfo.getFileSize();
    }

    return -1;
}


int64_t File::read(void *buf, size_t size)
{
    if (opened()) {
        return fread(buf, 1, size, mFileDesc);
    }

    return -1;
}

int64_t File::readall(ByteBuffer &buf)
{
    if (opened() == false) {
        return -1;
    }

    if (mFileInfo.getFileSize() > 1024 * 1024 * 1024) {
        throw Exception("the file is too large");
    }

    size_t cap = buf.size() + mFileInfo.getFileSize();
    if (cap > buf.capacity()) {
        buf.resize(cap);
    }

    uint8_t cache[1024] = {0};
    size_t ret = 0;
    int64_t count = 0;
    while (!feof(mFileDesc)) {
        ret = fread(cache, 1, sizeof(cache), mFileDesc);
        if (ret) {
            buf.append(cache, ret);
        }
        count += ret;
    }

    return count;
}

int64_t File::readline(ByteBuffer &buf)
{
    if (opened() == false) {
        return -1;
    }

    size_t count = 0;
    uint8_t ch;
    while (fread(&ch, 1, 1, mFileDesc)) {
        if (ch == '\n') { // unix
            return count;
        }

        if (ch == '\r' && fread(&ch, 1, 1, mFileDesc)) {
            if (ch == '\n') { // windows
                return count;
            } else {
                fseek(mFileDesc, -1, SEEK_CUR);
            }
        }

        ++count;
        buf.append(&ch, 1);
    } // 只有一行且没有换行符

    return count;
}

int64_t File::readline(String8 &buf)
{
    if (opened() == false) {
        return -1;
    }

    size_t count = 0;
    char ch;
    while (fread(&ch, 1, 1, mFileDesc)) {
        if (ch == '\n') { // unix
            return count;
        }

        if (ch == '\r' && fread(&ch, 1, 1, mFileDesc)) {
            if (ch == '\n') { // windows
                return count;
            } else {
                fseek(mFileDesc, -1, SEEK_CUR);
            }
        }

        ++count;
        buf.append(&ch, 1);
    } // 只有一行且没有换行符

    return count;
}

int64_t File::write(const void *buf, size_t size)
{
    if (opened()) {
        return fwrite(buf, 1, size, mFileDesc);
    }
    
    return -1;
}

int64_t File::write(const ByteBuffer &buf)
{
    if (opened()) {
        return fwrite(buf.const_data(), 1, buf.size(), mFileDesc);
    }
    
    return -1;
}

int64_t File::write(const String8 &buf)
{
    if (opened()) {
        return fwrite(buf.c_str(), 1, buf.length(), mFileDesc);
    }
    
    return -1;
}

} // namespace eular
