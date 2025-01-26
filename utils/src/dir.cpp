/*************************************************************************
    > File Name: dir.cpp
    > Author: hsz
    > Brief:
    > Created Time: 2024年11月21日 星期四 17时15分50秒
 ************************************************************************/

#include "utils/dir.h"

#include <sstream>
#include <deque>
#include <vector>

#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>

#include "utils/sysdef.h"
#include "utils/debug.h"

namespace eular {
namespace dir {
bool exists(const String8 &path)
{
    if (path.empty()) {
        return false;
    }

    struct stat lst;
    int32_t ret = lstat(path.c_str(), &lst);
    return ret == 0;
}

bool __mkdir(const char *path)
{
    if (access(path, F_OK) == 0) {
        return true;
    }

    return 0 == ::mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

std::vector<String8> _splitPath(const String8& path) {
    std::vector<String8> result;
    std::stringstream ss(path.c_str());
    
    std::string segment;
    while (std::getline(ss, segment, DIR_SEPARATOR)) {
        if (!segment.empty()) {
            result.push_back(segment);
        }
    }

    return result;
}

bool absolute(const String8 &path, String8 &absPath)
{
    if (path.empty()) {
        return false;
    }

    std::deque<String8> pathDeque;
    pathDeque.push_back("/");

    String8 realPath = path;
    if (path.front() == '~') {
        struct passwd result;
        struct passwd *pw = nullptr;
        char buffer[1204] = {0};
        int32_t code = getpwuid_r(getuid(), &result, buffer, sizeof(buffer), &pw);
        if (code != 0 || pw == nullptr) {
            LOG("getpwuid error: [%d,%s]", errno, strerror(errno));
            return false;
        }

        realPath = pw->pw_dir;
        realPath.append(path.c_str() + 1);
    }

    if (realPath.front() != '/') {
        return false;
    }

    if (realPath.find('.') < 0 && realPath.find("..") < 0) { // 不存在相对路径符号
        absPath = realPath;
        return true;
    }

    std::vector<String8> segments = _splitPath(realPath);
    for (size_t i = 0; i < segments.size(); ++i) {
        if (segments[i] == ".") {
            // do nothing
        } else if (segments[i] == "..") {
            if (pathDeque.size() > 1) { // 保留 跟路径 /
                pathDeque.pop_back();
            }
        } else {
            pathDeque.push_back(segments.at(i));
        }
    }

    absPath.reserve(path.length());
    while (!pathDeque.empty()) {
        absPath += pathDeque.front();
        if (absPath.back() != DIR_SEPARATOR) {
            absPath.append(DIR_SEPARATOR);
        }
        pathDeque.pop_front();
    }

    return true;
}

bool mkdir(const String8 &path)
{
    if (exists(path)) {
        return true;
    }

    String8 realPath;
    if (!absolute(path, realPath)) {
        return false;
    }

    char* filePath = strdup(realPath.c_str());
    char* ptr = strchr(filePath + 1, DIR_SEPARATOR);
    do {
        for (; ptr; *ptr = DIR_SEPARATOR, ptr = strchr(ptr + 1, DIR_SEPARATOR)) {
            *ptr = '\0';
            if (__mkdir(filePath) != 0) {
                break;
            }
        }
        if (ptr != nullptr) {
            break;
        } else if (!__mkdir(filePath)) {
            break;
        }
        free(filePath);
        return true;
    } while(0);

    free(filePath);
    return false;
}

}
// namespace dir
} // namespace eular
