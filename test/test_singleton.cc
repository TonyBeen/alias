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
        assert(*obj == std::string("12345"));
        printf("str addr = %p\n", (std::string *)obj);
    }

    {
        Singleton<eular::String8>::SObject obj = Singleton<eular::String8>::get("67890", 4);
        cout << *obj << endl;
        assert(*obj == eular::String8("6789"));
        printf("str addr = %p\n", (eular::String8 *)obj);

        Singleton<eular::String8>::SObject obj2 = Singleton<eular::String8>::reset("-------");
        cout << *obj2 << endl;
        assert(*obj == eular::String8("6789"));
        printf("str addr = %p\n", (eular::String8 *)obj2);

        assert(*obj == *obj2);
    }

    Singleton<eular::String8>::SObject obj = Singleton<eular::String8>::reset("-------");
    assert(*obj == "-------");
}

void test_free_buffer()
{
    class Test {
    public:
        Test() { std::cout << __func__ << "()\n"; }
        ~Test() { std::cout << __func__ << "()\n"; }
    };

    Singleton<Test>::get();
}

int main(int argc, char **argv)
{
    test_singleton();
    test_free_buffer();
    return 0;
}
