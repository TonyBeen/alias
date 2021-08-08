/*************************************************************************
    > File Name: testlist.cpp
    > Author: hsz
    > Mail:
    > Created Time: 2021年03月28日 星期日 13时54分22秒
 ************************************************************************/

#include <iostream>
#include "singlelist.h"
using namespace std;
using namespace Alias;

void print(void *data)
{
    const int *num = static_cast<int *>(data);
    cout << *num << endl;
}

int main(int argc, char **argv)
{
    Alias::SingleList<int> sl;
    sl.InsertBack(10);
    sl.InsertBack(20);
    sl.InsertBack(30);
    sl.InsertBack(40);

    sl.InsertFront(0);
    sl.DeleteFront();
    sl.DeleteBack();
    sl.DeleteByData(30);
    sl.InsertByPos(30, 1);
    sl.ForeachList(print);
    cout << "*********************\n";
    cout << "list size = " << sl.getSize() << endl;
    Alias::SingleList<int> tmp = sl;
    cout << "*********************\n";
    cout << "list size = " << tmp.getSize() << endl;
    return 0;
}
