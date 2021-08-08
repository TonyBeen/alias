/*************************************************************************
    > File Name: utils.cpp
    > Author: hsz
    > Mail:
    > Created Time: Wed May  5 15:00:59 2021
 ************************************************************************/

#include "utils.h"
#include <string.h>
#include <stdlib.h>


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

pid_t GetThreadId()
{
    return syscall(__NR_gettid);
}