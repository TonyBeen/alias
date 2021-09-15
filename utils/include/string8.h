/*************************************************************************
    > File Name: string8.h
    > Author: hsz
    > Mail:
    > Created Time: 2021年04月11日 星期日 12时24分52秒
 ************************************************************************/

#ifndef __STRING8_H__
#define __STRING8_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>
#include <string>

namespace Jarvis {

#define MAXSIZE (1024 * 8)

class String8 {
public:
                        String8();
                        String8(const String8& other);
                        String8(const char* other);
                        String8(const char* other, const size_t numChars);
                        String8(const std::string& other);
                        String8(String8 &&other);
                        ~String8();

    const char*         c_str() const;
    char *              data();
    String8             left(uint32_t count) const;     // 从左拷贝count个字符
    String8             right(uint32_t n) const;        // 从右拷贝n个字符
    void                trim(char c = ' ');             // 把字符串前后空格去掉
    void                trimLeft(char c = ' ');         // 去掉字符串左侧空格
    void                trimRight(char c = ' ');        // 去掉字符串右侧空格
    void                reverse();                      // 翻转字符串
    std::string         toStdString() const;            // String8 -> std::string

    bool                isEmpty() const;
    size_t              length() const;
    size_t              capacity() const { return mCapacity; }
    void                clear();

    int                 append(const String8& other);
    int                 append(const char* other);
    int                 append(const char* other, size_t numChars);

    static String8      format(const char* fmt, ...) __attribute__((format(printf, 1, 2)));         // this只能用于非静态成员函数，所以第一个参数就是fmt
    int                 appendFormat(const char* fmt, ...) __attribute__((format (printf, 2, 3)));  // 类成员函数第一个参数是隐藏的this指针

    String8&            operator=(const String8& other);
    String8&            operator=(const char* other);
    String8&            operator=(String8&& other);

    String8&            operator+=(const String8& other);
    String8             operator+(const String8& other) const;
    String8&            operator+=(const char* other);
    String8             operator+(const char* other) const;

    int                 compare(const String8& other) const;
    int                 compare(const char* other) const;
    int                 ncompare(const String8& other, size_t n) const;
    int                 ncompare(const char* other, size_t n) const;
    // if == return 0;
    int                 StrCaseCmp(const String8& other) const;
    int                 StrCaseCmp(const char* other) const;

    bool                operator<(const String8& other) const;
    bool                operator<=(const String8& other) const;
    bool                operator==(const String8& other) const;
    bool                operator!=(const String8& other) const;
    bool                operator>=(const String8& other) const;
    bool                operator>(const String8& other) const;

    bool                operator<(const char* other) const;
    bool                operator<=(const char* other) const;
    bool                operator==(const char* other) const;
    bool                operator!=(const char* other) const;
    bool                operator>=(const char* other) const;
    bool                operator>(const char* other) const;
    char&               operator[](size_t index);

    // return the index of the first byte of other in this at or after
    // start, or -1 if not found
    int32_t             find(const String8 &other, size_t start = 0) const;
    int32_t             find(const char* other, size_t start = 0) const;
    int32_t             find(const char c, size_t start = 0) const;
    // 找到第一个和最后一个c出现的位置，未找到则不更改begin和end值
    void                findChar(int &begin, int &end, char c = ' ') const;
    // 找到第一个不为c的字符和最后一个不为c的字符，如c = ' '，str = "  12345  ", begin = 2，end = 6
    void                findNotChar(int &begin, int &end, char c = ' ') const;
    // 返回最后一个字符串符合的位置,失败返回-1
    int32_t             find_last_of(const char *str) const;
    int32_t             find_last_of(const String8 &str) const;

    // return true if this string contains the specified substring
    bool                contains(const char* other) const;
    bool                removeAll(const char* other);

    void                toLower();
    void                toLower(size_t start, size_t numChars);
    void                toUpper();
    void                toUpper(size_t start, size_t numChars);
    // 未匹配到或参数错误返回负值，否则返回匹配到的字符串位置
    static int32_t      kmp_strstr(const char *val, const char *key);

private:
    friend std::ostream&   operator<<(std::ostream &out, const String8& in);
    char*               getBuffer(size_t numChars = 0);
    void                release();
    static String8      formatV(const char* fmt, va_list args);
    int                 appendFormatV(const char* fmt, va_list args);
    void                setTo(const String8& other);
    int                 setTo(const char* other);
    int                 setTo(const char* other, size_t numChars);
    int                 stringcompare(const char *other) const;
    // kmp next数组获取，非-1版本
    static int          getNext(String8 key, int n);

private:
    char *mString = nullptr;
    uint32_t    mCapacity;
};

// 为了方便输出
std::ostream&   operator<<(std::ostream &out, const String8& in);

} // namespace Jarvis

#endif // __STRING8_H__
