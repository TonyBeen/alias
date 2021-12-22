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

using namespace eular;
using namespace std;

void test_singleton()
{
    std::string *str = Singleton<std::string>::get("12345");
    cout << *str << endl;
    printf("str addr = %p\n", str);

    eular::String8 *str8 = Singleton<eular::String8>::get("67890", 4);
    cout << *str8 << endl;
    printf("str8 addr = %p\n", str8);

    eular::String8 *str8_t = Singleton<eular::String8>::reset("-------");
    cout << *str8_t << endl;
    printf("str8 addr = %p\n", str8_t);

    Singleton<std::string>::free();
    Singleton<eular::String8>::free();
}

int main(int argc, char **argv)
{
    test_singleton();
    return 0;
}
