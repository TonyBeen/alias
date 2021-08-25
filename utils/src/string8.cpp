/*************************************************************************
    > File Name: string8.cpp
    > Author: hsz
    > Mail:
    > Created Time: 2021年04月11日 星期日 12时25分02秒
 ************************************************************************/

#include "string8.h"
#include <errno.h>
#include <error.h>

namespace Alias {

char *String8::getBuffer(size_t numChars)
{
    char *ret = nullptr;
    if (numChars <= 0) {
        ret = (char *)malloc(1);
        if (ret) {
            *ret = '\0';
        }
        return ret;
    } else if (numChars > 0) {
        ret = (char *)malloc(numChars + 1);
        if (ret) {
            memset(ret, 0, numChars + 1);
        }
        return ret;
    }
}

void String8::release()
{
    if (mString == nullptr) {
        return;
    }
    free(mString);
    mString = nullptr;
}

String8::String8()
{
    mString = getBuffer();
}

String8::String8(const String8& other)
{
    if (other.length() == 0) {
        mString = getBuffer();
        return;
    }
    mString = getBuffer(other.length());
    memcpy(mString, other.mString, other.length());
}

String8::String8(const char* other)
{
    if (other == nullptr) {
        mString = getBuffer();
        return;
    }
    size_t length = strlen(other);
    mString = getBuffer(length);
    memcpy(mString, other, length);
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
    memcpy(mString, other, length);
}

String8::String8(const std::string& other)
{
    size_t length = other.length();
    mString = getBuffer(length);
    memcpy(mString, other.c_str(), length);
}

String8::String8(String8 &&other)
{
    if (other.mString == nullptr) {
        mString = getBuffer();
        return;
    }
    mString = other.mString;
    other.mString = nullptr;
}

String8::~String8()
{
    release();
}
size_t String8::find(const char* other, size_t start) const
{
    const char *tmp = strstr(mString + start, other);
    return tmp ? tmp - mString : -1;
}

const char* String8::c_str() const
{
    return mString;
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

    size_t totalSize = size + length();
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
    return 0;
}

String8& String8::operator=(const String8& other)
{
    setTo(other);
    return *this;
}
String8& String8::operator=(const char* other)
{
    setTo(other);
    return *this;
}

String8& String8::operator=(String8&& other)
{
    this->mString = other.mString;
    other.mString = nullptr;
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
String8  String8::operator+(const char* other) const
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

char& String8::operator[](size_t index)
{
    if (index >= length()) {    // 如果mString为null则返回0，故无需再判断mString
        return mString[0];
    }
    return mString[index];
}

int String8::stringcompare(const char *other) const
{
    if (!(mString || other)) {
        return -0xFFFF;         // means param error
    }
    return strcmp(mString, other);
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
    if (other == nullptr) {
        return -1;
    }
    size_t otherLen = strlen(other);
    size_t stringLen = 0;
    numChars = numChars > otherLen ? otherLen : numChars;
    if (mString) {
        stringLen = strlen(mString);
    }

    if (stringLen > numChars) {
        memset(mString, 0, stringLen);
        memcpy(mString, other, numChars);
    } else {
        release();
        mString = getBuffer(numChars);
        memcpy(mString, other, numChars);
    }
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

String8 String8::format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    String8 result(formatV(fmt, args));

    va_end(args);
    return std::move(result);
}

String8 String8::formatV(const char* fmt, va_list args)
{
    String8 result;
    result.appendFormatV(fmt, args);
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
    int n;
    va_list tmp_args;
    va_copy(tmp_args, args);
    n = vsnprintf(nullptr, 0, fmt, tmp_args);
    va_end(tmp_args);

    if (n < 0) {
        return n;
    }

    if (n > 0) {
        size_t oldLength = length();
        if ((size_t)n > MAXSIZE - 1 ||
            oldLength > MAXSIZE - (size_t)n - 1) {
            return -1;
        }

        char *buf = (char *)malloc(oldLength + n);
        if (buf == nullptr) {
            return -1;
        }
        memcpy(buf, mString, oldLength);
        release();
        vsnprintf(buf + oldLength, n + 1, fmt, args);
        mString = buf;
    }
    return n;
}

std::ostream& operator<<(std::ostream &out, const String8& in)
{
    out << in.toStdString();
    return out;
}

} // namespace Alias
