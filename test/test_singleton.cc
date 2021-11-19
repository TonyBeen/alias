/*************************************************************************
    > File Name: test_singleton.cc
    > Author: hsz
    > Brief:
    > Created Time: Fri 19 Nov 2021 04:38:34 PM CST
 ************************************************************************/

#include <utils/singleton.h>
#include <utils/string8.h>
#include <string>
#include <iostream>

using namespace Jarvis;
using namespace std;

void test_singleton()
{
    std::string *str = Singleton<std::string>::get("12345");
    cout << *str << endl;
    printf("str addr = %p\n", str);

    Jarvis::String8 *str8 = Singleton<Jarvis::String8>::get("67890", 4);
    cout << *str8 << endl;
    printf("str8 addr = %p\n", str8);

    Jarvis::String8 *str8_t = Singleton<Jarvis::String8>::reset("-------");
    cout << *str8_t << endl;
    printf("str8 addr = %p\n", str8_t);

    Singleton<std::string>::free();
    Singleton<Jarvis::String8>::free();
}

int main(int argc, char **argv)
{
    test_singleton();
    return 0;
}
