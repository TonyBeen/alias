/*************************************************************************
    > File Name: test_utils.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 10 Jan 2022 09:49:33 AM CST
 ************************************************************************/

#include <utils/utils.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
    std::vector<std::string> fileArray = getdir("./");
    for (auto tmp : fileArray) {
        printf("%s\n", tmp.c_str());
    }
    return 0;
}
