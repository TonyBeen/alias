/*************************************************************************
    > File Name: test_xml_config.cpp
    > Author: hsz
    > Brief:
    > Created Time: Thu 12 Jan 2023 03:41:10 PM CST
 ************************************************************************/

#include <iostream>
#include "../XmlConfig.h"

using namespace std;

int main(int argc, char **argv)
{
    std::string xml = "./xml_test.xml";
    if (argc > 1) {
        xml = argv[1];
    }

    eular::XmlConfig xmlConfig;
    xmlConfig.loadFile(xml);
    xmlConfig.foreach();

    return 0;
}
