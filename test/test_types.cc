/*************************************************************************
    > File Name: test_types.cc
    > Author: hsz
    > Brief:
    > Created Time: Wed 07 Sep 2022 09:53:47 AM CST
 ************************************************************************/

#include <utils/types.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace eular;

int main(int argc, char **argv)
{
    {
        uint16s temp(0xff);
        uint16_t temp2(0xff);
        EXPECT_TRUE(temp == temp2);
        uint32_t temp32_t = 0x12345678;
        temp2 = temp32_t;
        temp = temp32_t; // operator T => uinr16_t;
        // printf("0x%x, 0x%x\n", (uint16_t)temp, temp2);
        EXPECT_TRUE(temp == temp2);
        EXPECT_TRUE(temp == 0x5678);
    }
    
    {
        uint16s temp_16 = 0xff;
        uint32s temp = 0xf;
        uint32_t temp2 = 0xf;
        temp = temp_16; // operator T   =>  uint32_t
        temp2 = temp;
        EXPECT_TRUE(temp == temp2);
    }

    {
        uint64s temp = 0xffff;
        uint64_t temp2 = 0xffff;
        EXPECT_TRUE(temp == temp2);
    }

    return 0;
}
