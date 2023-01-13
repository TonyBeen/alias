/*************************************************************************
    > File Name: test_rwmutex.cpp
    > Author: hsz
    > Brief:
    > Created Time: Thu 12 Jan 2023 11:02:37 AM CST
 ************************************************************************/

#include <iostream>
#include "../rwmutex.h"
#include <thread>
#include <unistd.h>

using namespace std;

int number = 0;
RWMutex gMutex;
std::atomic<bool> gExit(false);

void thread_1()
{
    while (gExit == false) {
        gMutex.rlock();
        std::cout << "read1() " << number << std::endl;
        gMutex.runlock();
        usleep(500 * 1000);
    }
}

void thread_2()
{
    while (gExit == false) {
        gMutex.rlock();
        std::cout << "read2() " << number << std::endl;
        gMutex.runlock();
        usleep(500 * 1000);
    }
}

int main(int argc, char **argv)
{
    std::thread th1(thread_1);
    std::thread th2(thread_2);
    for (int i = 0; i < 10; ++i) {
        gMutex.wlock();
        std::cout << "write() " << ++number << std::endl;
        gMutex.wunlock();
        usleep(500 * 1000);
    }

    gExit = true;
    th1.join();
    th2.join();

    return 0;
}
