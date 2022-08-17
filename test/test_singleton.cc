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
#include <assert.h>

using namespace eular;
using namespace std;

void test_singleton()
{
    {
        Singleton<std::string>::SObject obj = Singleton<std::string>::get("12345");
        cout << *obj << endl;
        printf("str addr = %p\n", (std::string *)obj);
    }

    {
        Singleton<eular::String8>::SObject obj = Singleton<eular::String8>::get("67890", 4);
        cout << *obj << endl;
        printf("str addr = %p\n", (eular::String8 *)obj);

        Singleton<eular::String8>::SObject obj2 = Singleton<eular::String8>::reset("-------");
        cout << *obj2 << endl;
        printf("str addr = %p\n", (eular::String8 *)obj2);

        assert(*obj == *obj2);
    }

    Singleton<eular::String8>::SObject obj = Singleton<eular::String8>::reset("-------");
    assert(*obj == "-------");

    Singleton<std::string>::free();
    Singleton<eular::String8>::free();
}

int main(int argc, char **argv)
{
    test_singleton();
    return 0;
}
