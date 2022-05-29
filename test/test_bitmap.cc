/*************************************************************************
    > File Name: test_bitmap.cc
    > Author: hsz
    > Brief:
    > Created Time: 2022-05-23 11:52:14 Monday
 ************************************************************************/

#include <utils/bitmap.h>
#include <iostream>
#include <assert.h>

using namespace std;
using namespace eular;

int main()
{
    BitMap bitmap;
    int index = 10;
    bitmap.set(index, true);
	assert(bitmap.at(index) == true);

	cout << "BitMap Capacity: " << bitmap.capacity() << endl;

	bitmap.resize(100);
	cout << "BitMap Capacity: " << bitmap.capacity() << endl;

	assert(bitmap.at(index) == true);
	bitmap.set(index, false);
	assert(bitmap.at(index) == false);

	index = 6000;
	bitmap.set(index, true);
	assert(bitmap.at(index) == false);

	bitmap.set(1, true);
	assert(bitmap.at(1) == true);
	bitmap.clear();
	assert(bitmap.at(1) == false);

	printf("test success\n");
    return 0;
}