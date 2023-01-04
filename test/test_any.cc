/*************************************************************************
    > File Name: test_any.cc
    > Author: hsz
    > Brief:
    > Created Time: Fri 09 Dec 2022 03:52:20 PM CST
 ************************************************************************/

#include <iostream>
#include <utils/any.hpp>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    eular::Any any(std::string("Hello"));
    std::string str = eular::any_cast<std::string>(any);
    std::cout << str << std::endl;
    assert(str == "Hello");
    return 0;
}
