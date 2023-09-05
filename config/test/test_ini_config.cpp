/*************************************************************************
    > File Name: test_ini_config.cpp
    > Author: hsz
    > Brief:
    > Created Time: Fri 13 Jan 2023 11:29:11 AM CST
 ************************************************************************/

#include "../IniConfig.h"
#include <iostream>
#include <assert.h>

using namespace std;

#define CONFIG_FILE_PATH "test/ini_config.ini"

int main(int argc, char **argv)
{
    std::string path = CONFIG_FILE_PATH;
    if (argc == 2) {
        path = argv[1];
    }

    eular::IniConfig config;
    if (config.parser(path)) {
        printf("parser success\n\n");
        config["host"] = "192.168.20.23";
        assert(config["host"] == "192.168.20.23");
        assert(config.value("node1.host") == "192.168.10.12");
        config.keep("test/ini_out.ini");
    }

    return 0;
}
