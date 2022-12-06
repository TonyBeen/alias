/*************************************************************************
    > File Name: test_map.cc
    > Author: hsz
    > Brief:
    > Created Time: Mon 05 Dec 2022 09:58:35 AM CST
 ************************************************************************/

#include <utils/map.h>
#include <utils/string8.h>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <gtest/gtest.h>

using namespace std;

int main(int argc, char **argv)
{
    eular::Map<eular::String8, int> mapObj;
    std::cout << "test map insert\n";
    std::cout << "\tinsert to map: [\"hello\", 100]\n";
    std::cout << "\tinsert to map: [\"world\", 200]\n";
    std::cout << "\tinsert to map: [\"world\", 300] (will failed)\n";
    mapObj.insert("hello", 100);
    mapObj.insert("world", 200);
    mapObj.insert("world", 300); // will failed
    EXPECT_EQ(mapObj.size(), 2);

    {
        std::cout << "test map find.\n";
        auto it = mapObj.find("hello");
        EXPECT_TRUE(it != mapObj.end());
    }
   
    {
        std::cout << "test map value.\n";
        auto val = mapObj.value("hello");
        EXPECT_EQ(val, 100);
    }

    {
        std::cout << "test map operator[]\n";
        auto &val = mapObj["hello"];
        EXPECT_EQ(val, 100);
        val = 400;
        EXPECT_EQ(mapObj.value("hello"), 400);
    }

    {
        std::cout << "test map erase.\n";
        auto it = mapObj.insert("test", 10000);
        EXPECT_TRUE(it != mapObj.end());
        mapObj.erase("test");
        EXPECT_EQ(mapObj.size(), 2);
    }

    {
        std::cout << "test map foreach.\n";
        for (auto it = mapObj.begin(); it != mapObj.end(); ++it) {
            std::cout << "\tkey = " << it.key() << ", value = " << it.value() << std::endl; 
        }
        std::cout << "test map foreach and erase.\n";
        mapObj.insert("test", 1000);
        for (auto it = mapObj.begin(); it != mapObj.end(); ) {
            std::cout << "\tkey = " << it.key() << ", value = " << it.value() << std::endl;
            if (it.key() == "test") {
                it = mapObj.erase(it);
            } else {
                ++it;
            }
        }
        EXPECT_EQ(mapObj.size(), 2);
    }

    {
        std::cout << "test map reforeach.\n";
        for (auto it = mapObj.rbegin(); it != mapObj.rend(); --it) {
            std::cout << "\tkey = " << it.key() << ", value = " << it.value() << std::endl; 
        }
    }

    {
        std::cout << "test map copy.\n";
        auto mapNew = mapObj;
        mapNew.insert("new", 600);
        std::cout << "insert to new map [\"new\", 600] and foreach new map\n";
        eular::Map<eular::String8, int>::const_iterator cit;
        for (cit = mapNew.begin(); cit != mapNew.end(); ++cit) {
            std::cout << "\tkey = " << cit.key() << ", value = " << cit.value() << std::endl; 
        }
        std::cout << "foreach old map\n";
        for (auto it = mapObj.begin(); it != mapObj.end(); ++it) {
            std::cout << "\tkey = " << it.key() << ", value = " << it.value() << std::endl; 
        }
    }

    {
        std::cout << "test map assign\n";
        eular::Map<eular::String8, int> mapNew;
        mapNew = mapObj;
        mapNew.insert("new", 600);
        std::cout << "insert to new map [\"new\", 600] and foreach new map\n";
        eular::Map<eular::String8, int>::const_iterator cit;
        for (cit = mapNew.begin(); cit != mapNew.end(); ++cit) {
            std::cout << "\tkey = " << cit.key() << ", value = " << cit.value() << std::endl; 
        }
        std::cout << "foreach old map\n";
        for (auto it = mapObj.begin(); it != mapObj.end(); ++it) {
            std::cout << "\tkey = " << it.key() << ", value = " << it.value() << std::endl; 
        }
    }

    {
        std::cout << "test map clear.\n";
        mapObj.clear();
        EXPECT_EQ(mapObj.size(), 0);
    }

    return 0;
}
