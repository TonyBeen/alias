#include <iostream>
#include <stdio.h>
#include <log/log.h>
#include <utils/string8.h>

#define LOG_TAG "String8 test"

eular::String8 getString()
{
    eular::String8 tmp = "11111111111111";
    printf("getString() tmp.mString ptr = %p\n", tmp.c_str());
    return std::move(tmp);
}

int main()
{
    // 测试移动构造和移动赋值函数
    eular::String8 tmp = "1234567890";
    printf("tmp.mString ptr = %p\n", tmp.c_str());
    eular::String8 str1 = std::move(tmp);   // 此处走的移动构造
    printf("str1.mString ptr = %p\n", str1.c_str());
    std::cout << "str1: " << str1.toStdString() << std::endl;

    std::cout << "----------------------------------------\n";

    printf("tmp.mString ptr = %p\n", tmp.c_str());
    tmp = "abcdefg";
    printf("tmp.mString ptr = %p\n", tmp.c_str());
    eular::String8 str2;
    str2 = std::move(tmp);  // 此处走的移动赋值
    printf("str2.mString ptr = %p\n", str2.c_str());
    std::cout << "str2: " << str2.toStdString() << std::endl;

    eular::String8 str3;
    str3 = getString();
    printf("str3.mString ptr = %p\n", str3.c_str());
    std::cout << "str3: " << str3.toStdString() << std::endl;

    return 0;
}
