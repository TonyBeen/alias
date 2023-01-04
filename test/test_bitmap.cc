/*************************************************************************
    > File Name: test_bitmap.cc
    > Author: hsz
    > Brief:
    > Created Time: 2022-05-23 11:52:14 Monday
 ************************************************************************/

#include <utils/bitmap.h>
#include <gtest/gtest.h>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace eular;

int main()
{
    BitMap bitmap;
    int index = 10;
    bitmap.set(index, true);
    EXPECT_TRUE(bitmap.at(index));
    cout << "BitMap Capacity: " << bitmap.capacity() << endl;

    bitmap.resize(100);
    cout << "BitMap Capacity: " << bitmap.capacity() << endl;

    EXPECT_TRUE(bitmap.at(index));
    bitmap.set(index, false);
    EXPECT_FALSE(bitmap.at(index));

    index = 6000;
    bitmap.set(index, true);
    EXPECT_FALSE(bitmap.at(index));

    bitmap.set(1, true);
    EXPECT_TRUE(bitmap.at(1));
    bitmap.clear();
    EXPECT_FALSE(bitmap.at(1));

    return 0;
}