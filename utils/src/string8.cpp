/*************************************************************************
    > File Name: string8.cpp
    > Author: hsz
    > Mail:
    > Created Time: 2021年04月11日 星期日 12时25分02秒
 ************************************************************************/

#include "string8.h"
#include "debug.h"
#include "Errors.h"
#include "exception.h"
#include <error.h>

#define DEFAULT_STRING_SIZE 128
#define MAXSIZE (1024 * 8)

namespace eular {
char *String8::getBuffer(size_t numChars)
{
    char *buf = nullptr;
    if (numChars < DEFAULT_STRING_SIZE) {   // 小于默认字符串长度
        buf = (char *)malloc(DEFAULT_STRING_SIZE);
        mCapacity = DEFAULT_STRING_SIZE;
    } else if (numChars < MAXSIZE / 2) {    // 小于最大长度的二分之一就申请1.5倍
        buf = (char *)malloc(numChars / 2 * 3);
        mCapacity = numChars / 2 * 3;
    } else if (numChars < MAXSIZE) {        // 大于最大长度二分之一小于最大长度，就申请最大长度
        buf = (char *)malloc(MAXSIZE);
        mCapacity = MAXSIZE;
    } else {                                // 大于最大长度，抛出异常
        throw Exception("Too many characters");
    }

    if (buf) {
        memset(buf, 0, mCapacity);
    } else {
        mCapacity = 0;
    }
    return buf;
}

void String8::release()
{
    if (mString) {
        free(mString);
    }
    mString = nullptr;
    mCapacity = 0;
}

String8::String8()
{
    mString = getBuffer();
}

String8::String8(uint32_t size) :
    mString(nullptr)
{
    mString = (char *)malloc(size);
    if (mString) {
        memset(mString, 0, size);
        mCapacity = size;
    }
}

String8::String8(const String8& other)
{
    if (other.length() == 0) {
        mString = getBuffer();
        return;
    }
    mString = getBuffer(other.length());
    if (mString) {
        memcpy(mString, other.mString, other.length());
    }
}

String8::String8(const char* other)
{
    if (other == nullptr) {
        mString = getBuffer();
        return;
    }
    size_t length = strlen(other);
    mString = getBuffer(length);
    if (mString) {
        memcpy(mString, other, length);
    }
}

String8::String8(const char* other, const size_t numChars)
{
    if (other == nullptr) {
        mString = getBuffer();
        return;
    }
    size_t length = strlen(other);
    if (length > numChars) {
        length = numChars;
    }
    mString = getBuffer(length);
    if (mString) {
        memcpy(mString, other, length);
    }
}

String8::String8(const std::string& other)
{
    size_t length = other.length();
    mString = getBuffer(length);
    if (mString) {
        memcpy(mString, other.c_str(), length);
    }
}

String8::String8(String8 &&other) :
    mString(nullptr),
    mCapacity(0)
{
    if (other.mString == nullptr) {
        mString = getBuffer();
        return;
    }
    mString = other.mString;
    other.mString = nullptr;
    mCapacity = other.mCapacity;
}

String8::~String8()
{
    release();
}

int32_t String8::find(const String8 &other, size_t start) const
{
    const char *tmp = strstr(mString + start, other.mString);
    return tmp ? tmp - mString : -1;
}

int32_t String8::find(const char* other, size_t start) const
{
    const char *tmp = strstr(mString + start, other);
    return tmp ? tmp - mString : -1;
}

int32_t String8::find(const char c, size_t start) const
{
    const char *index = strchr(mString + start, c);
    return index ? index - mString : -1;
}

const char* String8::c_str() const
{
    return mString;
}

char *String8::data()
{
    return mString;
}

String8 String8::left(uint32_t count) const
{
    char *buf = new char[count + 1]{ 0 };
    if (buf == nullptr) {
        return String8("");
    }

    strncpy(buf, mString, count);
    String8 str = buf;
    delete [] buf;
    return std::move(str);
}
String8 String8::right(uint32_t n) const
{
    char *buf = new char[n + 1]{ 0 };
    if (buf == nullptr) {
        return String8("");
    }

    strncpy(buf, mString + (length() - n), n);
    String8 str = buf;
    delete [] buf;
    return std::move(str);
}

void String8::trim(char c)
{
    int begin = 0;
    int end = 0;
    findNotChar(begin, end, c);
    String8 tmp(mString + begin, end - begin);

    char *swap = nullptr;
    swap = tmp.mString;
    tmp.mString = this->mString;
    this->mString = swap;
}

void String8::trimLeft(char c)
{
    int begin = 0;
    int end = 0;
    findNotChar(begin, end, c);
    String8 tmp(mString + begin);

    char *swap = nullptr;
    swap = tmp.mString;
    tmp.mString = this->mString;
    this->mString = swap;
}

void String8::trimRight(char c)
{
    int begin = 0;
    int end = 0;
    findNotChar(begin, end, c);
    String8 tmp(mString, end + 1);

    char *swap = nullptr;
    swap = tmp.mString;
    tmp.mString = this->mString;
    this->mString = swap;
}

void String8::reverse()
{
    if (length() == 0) {
        return;
    }
    char *buf = getBuffer(length());
    if (buf == nullptr) {
        return;
    }
    for (size_t i = 0; i < length(); ++i) {
        buf[i] = mString[length() - 1 - i];
    }
    free(mString);
    mString = buf;
}

void String8::resize(size_t size)
{
    if (length() >= size) {
        return;
    }
    String8 temp(size);
    temp = mString;
    release();
    std::move(temp);
}

std::string String8::toStdString() const
{
    return std::string(mString);
}

bool String8::isEmpty() const
{
    return length() == 0;
}

size_t String8::length() const
{
    size_t len = 0;
    if (mString) {
        len = strlen(mString);
    }
    return len;
}

void String8::clear()
{
    if (mString) {
        memset(mString, 0, strlen(mString));
    }
}

int String8::append(const String8& other)
{
    return append(other.mString, other.length());
}

int String8::append(const char* other)
{
    return append(other, strlen(other));
}

int String8::append(const char* other, size_t numChars)
{
    if (other == nullptr) {
        return -1;
    }
    size_t size = strlen(other);
    if (size > numChars) {
        size = numChars;
    }
    if (size == 0) {
        return 0;
    }

    size_t oldLen = length();
    size_t totalSize = size + oldLen;
    if (totalSize < mCapacity) {
        if (mString) {
            memmove(mString + length(), other, size);
            return size;
        }
    } else {
        char *buf = getBuffer(totalSize);
        if (buf) {
            if (mString) {
                memcpy(buf, mString, length());
            }
            memcpy(buf + length(), other, size);
            this->release();
            mString = buf;
            return size;
        }
    }
    
    return 0;
}

String8& String8::operator=(const String8& other)
{
    setTo(other.c_str(), other.length());
    return *this;
}

String8& String8::operator=(const char* other)
{
    setTo(other);
    return *this;
}

String8& String8::operator=(String8&& other)
{
    char *tmp = this->mString;
    this->mString = other.mString;
    other.mString = tmp;
    mCapacity = other.mCapacity;
    other.mCapacity = 0;
    return *this;
}

String8& String8::operator+=(const String8& other)
{
    this->append(other);
    return *this;
}
String8 String8::operator+(const String8& other) const
{
    String8 tmp(*this);
    tmp += other;
    return std::move(tmp);
}
String8& String8::operator+=(const char* other)
{
    this->append(other);
    return *this;
}
String8 String8::operator+(const char* other) const
{
    String8 tmp(*this);
    tmp += other;
    return std::move(tmp);
}

int String8::compare(const String8& other) const
{
    return stringcompare(other.mString);
}

int String8::compare(const char* other) const
{
    return stringcompare(other);
}

int String8::ncompare(const String8& other, size_t n) const
{
    return strncmp(mString, other.mString, n);
}

int String8::ncompare(const char* other, size_t n) const
{
    if (other == nullptr) {
        return 1;
    }
    return strncmp(mString, other, n);
}

int String8::StrCaseCmp(const String8& other) const
{
    if (mString) {
        return strcasecmp(mString, other.mString);
    }
    return -EPERM;
}

int String8::StrCaseCmp(const char* other) const
{
    if (mString) {
        return strcasecmp(mString, other);
    }
    return -EPERM;
}

bool String8::operator<(const String8& other) const
{
    return stringcompare(other.mString) < 0;
}
bool String8::operator<=(const String8& other) const
{
    return stringcompare(other.mString) <= 0;
}
bool String8::operator==(const String8& other) const
{
    return stringcompare(other.mString) == 0;
}
bool String8::operator!=(const String8& other) const
{
    return stringcompare(other.mString) != 0;
}
bool String8::operator>=(const String8& other) const
{
    return stringcompare(other.mString) >= 0;
}
bool String8::operator>(const String8& other) const
{
    return stringcompare(other.mString) > 0;
}

bool String8::operator<(const char* other) const
{
    return stringcompare(other) < 0;
}
bool String8::operator<=(const char* other) const
{
    return stringcompare(other) <= 0;
}
bool String8::operator==(const char* other) const
{
    return stringcompare(other) == 0;
}
bool String8::operator!=(const char* other) const
{
    return stringcompare(other) != 0;
}
bool String8::operator>=(const char* other) const
{
    return stringcompare(other) >= 0;
}
bool String8::operator>(const char* other) const
{
    return stringcompare(other) > 0;
}

// 如果index超过范围则返回最后一个位置'\0'
char& String8::operator[](size_t index)
{
    if (index >= length()) {    // 如果mString为null则返回0，故无需再判断mString
        return mString[length()];
    }
    return mString[index];
}

int String8::stringcompare(const char *other) const
{
    if (!mString || !other) {
        return -0xFFFF;         // means param error
    }
    return strcmp(mString, other);
}

int String8::getNext(String8 key, int n)
{
    if (n < 2) {
        return 0;
    }
    if (n == 2) {
        if (key[0] == key[1]) {
            return 1;
        }
        return 0;
    }
    int max = 0;
    for (int k = 1; k < n; ++k) {
        if (strncmp(key.c_str() + n - k, key.c_str(), k) == 0) {
            max = k > max ? k : max;
        }
    }
    return max;
}

void String8::setTo(const String8& other)
{
    setTo(other.mString, other.length());
}

int String8::setTo(const char* other)
{
    size_t len = strlen(other);
    return setTo(other, len);
}

int String8::setTo(const char* other, size_t numChars)
{
    if (other == nullptr || numChars == 0) {
        return -1;
    }
    int ret = 0;

    size_t otherLen = strlen(other);
    numChars = numChars > otherLen ? otherLen : numChars;

    if (mString && mCapacity < numChars) {
        release();
        mString = getBuffer(numChars);
        if (mString) {
            memmove(mString, other, numChars);
            ret = numChars;
        }
    } else {
        if (!mString) {
            mString = getBuffer(numChars);
        }
        clear();
        memmove(mString, other, numChars);
        ret = numChars;
    }

    return ret;
}

void String8::findNotChar(int &begin, int &end, char c) const
{
    const int len = length();
    for (size_t i = 0; i < len; ++i) {
        if (mString[i] == c) {
            continue;
        }
        begin = i;
        break;
    }
    for (size_t i = len - 1; i > 0; --i) {
        if (mString[i] == c) {
            continue;
        }
        end = i;
        break;
    }
}

int32_t String8::find_last_of(const char *key) const
{
    // 思想，将两个字符串均翻转后在匹配，在使用字符串长度减去匹配字符串长度减查找到的字符串位置
    String8 keyStr = key;
    String8 value = mString;
    value.reverse();
    keyStr.reverse();
    int index = value.find(keyStr.c_str());
    if (index < 0) {
        return index;
    }
    return length() - index - keyStr.length();
}

int32_t String8::find_last_of(const String8 &str) const
{
    return find_last_of(str.c_str());
}

String8 String8::substr(size_t start, size_t end)
{
    String8 ret;
    if (start >= length() || end >= length() || start >= end) {
        return ret;
    }

    ret = String8(mString + start, end);
    return ret;
}

bool String8::contains(const char* other) const
{
    return nullptr != strstr(mString, other);
}

bool String8::removeAll(const char* other)
{
    size_t start = 0;
    if (other && ((start = find(other)) >= 0)) {
        size_t end = strlen(other) + start;
        if (end >= length()) {
            mString[start] = '\0';
            return true;
        } else {
            size_t bufferSize = length() - (end - start);
            char *newString = getBuffer(bufferSize);
            if (newString == nullptr) {
                return false;
            }
            memcpy(newString, mString, start);
            memcpy(newString + start, mString + end, length() - end);
            free(mString);
            mString = newString;
            return true;
        }
    }
    return false;
}

/**
 * @brief 将所有的o字符替换为n字符
 * 
 * @param o old char
 * @param n new char
 * @return ssize_t 
 */
ssize_t String8::replaceAll(char o, char n)
{
    if (n == o) {
        return 0;
    }

    ssize_t count = 0;
    for (size_t i = 0; i < length(); ++i) {
        if (mString[i] == o) {
            mString[i] = n;
            ++count;
        }
    }

    return count;
}

void String8::toLower()
{
    toLower(0, length());
}

void String8::toLower(size_t start, size_t numChars)
{
    size_t size = length();
    if (size > start) {
        for (size_t i = start; i < numChars && i < size; ++i) { // i < size -> anti overflow
            mString[i] = static_cast<char>(tolower(mString[i]));
        }
    }
}

void String8::toUpper()
{
    toUpper(0, length());
}

void String8::toUpper(size_t start, size_t numChars)
{
    size_t size = length();
    if (size > start) {
        for (size_t i = start; i < numChars && i < size; ++i) { // i < size -> anti overflow
            mString[i] = static_cast<char>(toupper(mString[i]));
        }
    }
}

int32_t String8::kmp_strstr(const char *val, const char *key)
{
    if (val == nullptr || key == nullptr) {
        return INVALID_PARAM;
    }
    int valLen = strlen(val);
    int keyLen = strlen(key);
    int i = 0;
    int j = 0;
    for (i = 0; i < valLen;) {
        for (j = 0; j < keyLen; ++j) {
            if (val[i + j] == key[j]) {
                continue;
            }
            // j > 0: 表示当前存在匹配上的一段字符串，但是不完全匹配，所以需要偏移
            if (j > 0) {
                i += (j - getNext(String8(key, j), j));
            } else { // 没有匹配到一个字符则只移动一个位置
                ++i;
            }
            break;
        }
        if (j == keyLen) { // 由continue跳出循环时需判断是否j == 子串长度
            return i;
        }
    }
    return -1;
}

String8 String8::format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    String8 result = formatV(fmt, args);

    va_end(args);
    return std::move(result);
}

String8 String8::formatV(const char* fmt, va_list args)
{
    String8 result;
    int len = 0;
    char *buf = nullptr;

    va_list tmp_args;
    va_copy(tmp_args, args);
    len = vsnprintf(nullptr, 0, fmt, tmp_args);
    va_end(tmp_args);

    if (len > 0) {
        buf = (char *)malloc(len + 1);
        if (buf != nullptr) {
            vsnprintf(buf, len + 1, fmt, args);
            result = buf;
            free(buf);
        }
    }

    return std::move(result);
}

int String8::appendFormat(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    int numChars = appendFormatV(fmt, args);

    va_end(args);
    return numChars;
}

int String8::appendFormatV(const char* fmt, va_list args)
{
    int n = 0;
    va_list tmp_args;
    va_copy(tmp_args, args);
    n = vsnprintf(nullptr, 0, fmt, tmp_args);
    va_end(tmp_args);
    if (n <= 0) {
        return n;
    }

    size_t oldLength = length();
    if ((oldLength + n) > MAXSIZE - 1) {
        return -1;
    }
    char *buf = nullptr;
    if (mCapacity < oldLength + n) {
        buf = getBuffer(oldLength + n);
        if (buf == nullptr) {
            return -1;
        }
    }

    if (buf) {
        memcpy(buf, mString, oldLength);
        vsnprintf(buf + oldLength, n + 1, fmt, args);   // 加1是包含最后的\0
        release();
        mString = buf;
        return n;
    }

    vsnprintf(mString + oldLength, n + 1, fmt, args);
    return n;
}

std::ostream& operator<<(std::ostream &out, const String8& in)
{
    out << in.toStdString();
    return out;
}

} // namespace eular
