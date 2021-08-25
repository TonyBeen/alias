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
#include <string>
#include <stdarg.h>
#include <stdint.h>
#include <ctype.h>

namespace Alias {

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
    std::string         toStdString() const;           // String8 -> std::string

    bool                isEmpty() const;
    size_t              length() const;
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
    size_t              find(const char* other, size_t start = 0) const;

    // return true if this string contains the specified substring
    bool                contains(const char* other) const;
    bool                removeAll(const char* other);

    void                toLower();
    void                toLower(size_t start, size_t numChars);
    void                toUpper();
    void                toUpper(size_t start, size_t numChars);

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
private:
    char *mString = nullptr;
};

// 为了方便输出
std::ostream&   operator<<(std::ostream &out, const String8& in);

} // namespace Alias

#endif // __STRING8_H__
