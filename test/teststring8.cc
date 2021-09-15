#include <iostream>
#include <stdio.h>
#include <log/log.h>
#include <utils/string8.h>

#define LOG_TAG "String8 test"

using namespace Jarvis;

int main()
{
    {
        Jarvis::String8 str1 = "123456789";
        str1.append("abcde", 3);
        std::cout << str1.c_str() << "\n";
        str1.appendFormat(" %p", str1.c_str());
        std::cout << "ptr = " << str1.c_str() << "\n";

        str1.clear();
        str1.appendFormat("[%d.%d.%d.%d]", 192, 168, 124, 7);
        std::cout << str1.c_str() << "\n";

        Jarvis::String8 str2 = str1;
        str2.appendFormat(" %p", str2.c_str());
        std::cout << "str2: "<< str2.c_str() << "\n";
        std::cout << str2.find("168.124.7") << "\n";
        std::cout << str2.contains("168.124.7") << "\n";
        std::cout << str2.compare(str1) << "\n";
        str2 = "ABcdEF";
        str2.toLower();
        std::cout << "toLower str2: "<< str2.c_str() << "\n";

        Jarvis::String8 str3(str1);
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
        Jarvis::String8 str4 = str1 + str2 + str3;
        std::cout << "str4: " << str4.toStdString() << std::endl;

        Jarvis::String8 str5 = Jarvis::String8::format("%s", str4.c_str());
        std::cout << "str5: " << str5.toStdString() << std::endl;
    }

    std::cout << "----------------------------------------\n";

    {
        // 测试移动构造和移动赋值函数
        std::cout << "测试移动构造和移动赋值函数\n";
        Jarvis::String8 tmp = "1234567890";
        printf("tmp.mString ptr = %p\n", tmp.c_str());
        Jarvis::String8 str1 = std::move(tmp);   // 此处走的移动构造
        printf("str1.mString ptr = %p\n", str1.c_str());
        std::cout << "str1: " << str1.toStdString() << std::endl;

        std::cout << "*************\n";

        printf("tmp.mString ptr = %p\n", tmp.c_str());
        tmp = "abcdefg";
        printf("tmp.mString ptr = %p\n", tmp.c_str());
        Jarvis::String8 str2;
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
        Jarvis::String8 str8 = "Hello World!";
        std::cout << "str8 = " << str8 << std::endl;
    }

    {
        // 测试left right operator[] trim trimLeft trimRight reverse
        Jarvis::String8 str1 = "127.0.0.1:8000";
        int32_t index = str1.find(':');
        std::cout << str1[index] << std::endl;
        std::cout << "A big number: " << str1[100] << std::endl;
        std::cout << (str1[str1.length()] == '\0' ? "true" : "false") << std::endl;
        if (index > 0) {
            std::cout << str1.left(index) << std::endl;
            std::cout << str1.right(str1.length() - (index + 1)) << std::endl;
        }
        Jarvis::String8 str2 = "\t\t12345\t\t";
        str2.trim('\t');
        std::cout << str2 << std::endl;

        Jarvis::String8 str3 = "\t12345";
        str3.trimLeft('\t');
        std::cout << str3 << std::endl;

        Jarvis::String8 str4 = "12345\n";
        str4.trimRight('\n');
        std::cout << "trimRight: " << str4 << std::endl;
        str4.reverse();
        std::cout << str4 << std::endl;
    }

    {
        // 测试 find_last_of kmp_strstr
        Jarvis::String8 str1 = "sssabcssdeabcss";
        int index = str1.find_last_of("abc");
        std::cout << "find_last_of: " << index << std::endl;
        LOG_ASSERT(index == 10, "index should equal to five\n");

        const char *val = "BBC ABCDAB ABCDABCDABDE";
        const char *key = "ABCDABD";
        bool flag = (Jarvis::String8::kmp_strstr(val, key) == strstr(val, key) - val);
        //LOG_ASSERT(false, "error");
    }

    {
        // 测试format
        std::cout << String8::format("-0x%012lx: (%s + 0x%lx)", 0x1110, "for test", 0x1110) << std::endl;
        String8 str1 = String8::format("-0x%012lx: (%s + 0x%lx)", 0x1110, "for test", 0x1110);
        std::cout << std::endl;
    }

    return 0;
}
