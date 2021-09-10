/*************************************************************************
    > File Name: utils.cpp
    > Author: hsz
    > Mail:
    > Created Time: Wed May  5 15:00:59 2021
 ************************************************************************/

#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>

nsec_t seconds(uint16_t sec)
{
    return sec * 1000 * 1000 * 1000;
}

nsec_t mseconds(uint16_t ms)
{
    return ms * 1000 * 1000;
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

int msleep(uint32_t ms)
{
    struct timeval time;
    int second = ms / 1000;
    int us = (ms - second * 1000) * 1000;
    time.tv_sec = ms / 1000;
    time.tv_usec = us;

    return select(0, NULL, NULL, NULL, &time);
}

// ps也是走的这种方法
#define BUF_SIZE 280
std::vector<int> getPidByName(const char *procName)
{
    std::vector<int> pidVec;
    if (procName == nullptr) {
        return std::move(pidVec);
    }
    DIR *dir = nullptr;
    struct dirent *ptr = nullptr;
    FILE *fp = nullptr;
    char fildPath[BUF_SIZE];
    char cur_task_name[32];
    char buf[BUF_SIZE];

    dir = opendir("/proc");
    if (nullptr != dir) {
        // 循环读取/proc下的每一个文件
        while ((ptr = readdir(dir)) != nullptr) {
            // 如果读取到的是"."或者".."则跳过，读取到的不是文件夹类型也跳过
            if ((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
                continue;
            if (DT_DIR != ptr->d_type)
                continue;

            // cmdlines存放的是启动进程时第一个入参，argv[0]
            snprintf(fildPath, BUF_SIZE - 1,"/proc/%s/cmdline", ptr->d_name);
            fp = fopen(fildPath, "r");
            if (nullptr != fp) {
                if (fgets(buf, BUF_SIZE - 1, fp) == nullptr) {
                    fclose(fp);
                    continue;
                }

                // %*s %s表示添加了*的字符串会忽略，后面的%s赋值给cur_task_name
                // sscanf(buf, "%*s %s", cur_task_name); // 读取status时需要格式化字符串
                if (strcmp(procName, buf) == 0) {
                    pidVec.push_back(atoi(ptr->d_name));
                }
                fclose(fp);
            }
        }
        closedir(dir);
    }
    return std::move(pidVec);
}

std::string getNameByPid(pid_t pid)
{
    if (pid <= 0) {
        return "";
    }

    FILE *fp = nullptr;
    char buf[BUF_SIZE] = {0};

    snprintf(buf, BUF_SIZE, "/proc/%d/status", pid);
    fp = fopen(buf, "r");
    if (fp == nullptr) {
        perror("fopen failed:");
        return "";
    }
    memset(buf, 0, BUF_SIZE);
    fgets(buf, BUF_SIZE, fp);
    if (buf[0] != '\0') {
        char name[BUF_SIZE] = {0};
        sscanf(buf, "%*s %s", name);
        return name;
    }
    return "";
}

pid_t GetThreadId()
{
    return syscall(__NR_gettid);
}

std::string Time2Str(time_t ts, const std::string& format)
{
    struct tm tm;
    localtime_r(&ts, &tm);
    char buf[64];
    strftime(buf, sizeof(buf), format.c_str(), &tm);
    return buf;
}

time_t Str2Time(const char* str, const char* format)
{
    struct tm t;
    memset(&t, 0, sizeof(t));
    if(!strptime(str, format, &t)) {
        return 0;
    }
    return mktime(&t);
}

int8_t  TypeUtil::ToChar(const std::string& str)
{
    if(str.empty()) {
        return 0;
    }
    return *str.begin();
}

int64_t TypeUtil::Atoi(const std::string& str)
{
    if(str.empty()) {
        return 0;
    }
    return strtoull(str.c_str(), nullptr, 10);
}

double  TypeUtil::Atof(const std::string& str)
{
    if(str.empty()) {
        return 0;
    }
    return atof(str.c_str());
}

int8_t  TypeUtil::ToChar(const char* str)
{
    if(str == nullptr) {
        return 0;
    }
    return str[0];
}

int64_t TypeUtil::Atoi(const char* str)
{
    if(str == nullptr) {
        return 0;
    }
    return strtoull(str, nullptr, 10);
}

double  TypeUtil::Atof(const char* str)
{
    if(str == nullptr) {
        return 0;
    }
    return atof(str);
}
