#include <iostream>
#include <stdio.h>
#include <log/log.h>
#include "../utils/string8.h"

#define LOG_TAG "String8 test"

int main()
{
    {
        Alias::String8 str1 = "123456789";
        str1.append("abcde", 3);
        std::cout << str1.c_str() << "\n";
        str1.appendFormat(" %p", str1.c_str());
        std::cout << "ptr = " << str1.c_str() << "\n";

        str1.clear();
        str1.appendFormat("[%d.%d.%d.%d]", 192, 168, 124, 7);
        std::cout << str1.c_str() << "\n";

        Alias::String8 str2 = str1;
        str2.appendFormat(" %p", str2.c_str());
        std::cout << "str2: "<< str2.c_str() << "\n";
        std::cout << str2.find("168.124.7") << "\n";
        std::cout << str2.contains("168.124.7") << "\n";
        std::cout << str2.compare(str1) << "\n";
        str2 = "ABcdEF";
        str2.toLower();
        std::cout << "toLower str2: "<< str2.c_str() << "\n";

        Alias::String8 str3(str1);
        std::cout << "str3: "<< str3.c_str() << "\n";
        std::cout << (str3 == str2) << "\n";
        str3 = "AbcdEF";
        std::cout << str3.StrCaseCmp(str2) << "\n";
        str3.toLower();
        std::cout << "toLower str3: "<< str3.c_str() << "\n";
        std::cout << "str3 == str2 -> " << (str3 == str2) << "\n";

        str3.removeAll("abcdef");
        str3 += str2;
        std::cout << "str3: "<< str3.c_str() << "\n";
        Alias::String8 str4 = str1 + str2 + str3;
        std::cout << "str4: " << str4.toStdString() << std::endl;

        Alias::String8 str5 = Alias::String8::format("%s", str4.c_str());
        std::cout << "str5: " << str5.toStdString() << std::endl;
    }

    std::cout << "----------------------------------------\n";

    {
        // 测试移动构造和移动赋值函数
        std::cout << "测试移动构造和移动赋值函数\n";
        Alias::String8 tmp = "1234567890";
        printf("tmp.mString ptr = %p\n", tmp.c_str());
        Alias::String8 str1 = std::move(tmp);   // 此处走的移动构造
        printf("str1.mString ptr = %p\n", str1.c_str());
        std::cout << "str1: " << str1.toStdString() << std::endl;

        std::cout << "*************\n";

        printf("tmp.mString ptr = %p\n", tmp.c_str());
        tmp = "abcdefg";
        printf("tmp.mString ptr = %p\n", tmp.c_str());
        Alias::String8 str2;
        str2 = std::move(tmp);  // 此处走的移动赋值
        printf("str2.mString ptr = %p\n", str2.c_str());
        std::cout << "str2: " << str2.toStdString() << std::endl;

        /*
            通过此次测试可以发现函数返回值是通过拷贝构造赋值给新的变量的，并非通过等号赋值函数
            另外，创建变量使用等号赋值也走的是构造函数，只有在声明变量后在赋值调用的是等号赋值函数
         */
    }

    {
        // 测试左移运算符
        Alias::String8 str8 = "Hello World!";
        std::cout << "str8 = " << str8 << std::endl;
    }

    return 0;
}
