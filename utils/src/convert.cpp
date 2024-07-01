/*************************************************************************
    > File Name: convert.cpp
    > Author: hsz
    > Brief:
    > Created Time: Thu 28 Jul 2022 02:54:16 PM CST
 ************************************************************************/

#include "utils/convert.h"

#include <string.h>
#include <assert.h>

#include "utils/utils.h"

bool convert(const std::string &fromCode, const std::string &toCode, std::string);

/**
 * UTF-8是一种多字节编码的字符集，表示一个Unicode字符时，它可以是1个至多个字节
 * 除了一字节外，左边1的个数表示当前编码字节数，utf8最多可以扩展到6个字节,中文字符占三个字节
 * 1字节：0xxxxxxx
 * 2字节：110xxxxx 10xxxxxx
 * 3字节：1110xxxx 10xxxxxx 10xxxxxx
 * 4字节：11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
 */

#define IS_UTF8_ANSII(code) (0x00 < (uint8_t)(code) && (uint8_t)(code) < 0x80)  // ansii字符集
#define IS_UTF8_2BYTE(code) (0xc0 < (uint8_t)(code) && (uint8_t)(code) < 0xe0)  // 此范围内为2字节UTF-8字符
#define IS_UTF8_3BYTE(code) (0xe0 < (uint8_t)(code) && (uint8_t)(code) < 0xf0)  // 此范围内为3字节UTF-8字符
#define IS_UTF8_4BYTE(code) (0xf0 < (uint8_t)(code) && (uint8_t)(code) < 0xf8)  // 此范围内为4字节UTF-8字符
#define IS_UTF8_EXTRA(code) (0x80 < (uint8_t)(code) && (uint8_t)(code) < 0xc0)  // 其余的字符规则

bool isUTF8Encode(const std::string &str)
{
    if (str.length() == 0) {
        return false;
    }
    const char *start = str.c_str();
    const char *end = start + str.length();
    bool isUtf8 = false;

    while (start < end) {
        if (IS_UTF8_ANSII(*start)) {
            ++start;
            continue;
        }
        if (IS_UTF8_2BYTE(*start)) {
            if ((start + 1) >= end) { // 当是utf8时，检测到2字节编码头时后面会跟着一个字符
                return false;
            }
            if (IS_UTF8_EXTRA(*(start + 1))) {
                isUtf8 = true;
            }
            start += 2;
            continue;
        }
        if (IS_UTF8_3BYTE(*start)) {
            if (start + 2 >= end) { // 检测到3字节编码头时，后面会有两个字节的位置
                return false;
            }
            if (IS_UTF8_EXTRA(*(start + 1)) && IS_UTF8_EXTRA(*(start + 2))) {
                isUtf8 = true;
            }
            start += 3;
            continue;
        }
        if (IS_UTF8_4BYTE(*start)) {
            if (start + 3 >= end) {
                return false;
            }
            if (IS_UTF8_EXTRA(*(start + 1)) && IS_UTF8_EXTRA(*(start + 2)) && IS_UTF8_EXTRA(*(start + 3))) {
                isUtf8 = true;
            }
            start += 4;
            continue;
        }
        isUtf8 = false;
        break;
    }

    return isUtf8;
}

int getChar(const char *str, size_t size)
{
    assert(str != nullptr && size != 0);
    const char *start = str;
    const char *end = start + size;
    if (IS_UTF8_ANSII(*start)) {
        return 0;
    }

    if (IS_UTF8_2BYTE(*start)) {
        if ((start + 1) >= end) { // 当是utf8时，检测到2字节编码头时后面会跟着一个字符
            return -1;
        }
        if (IS_UTF8_EXTRA(*(start + 1))) {
            return 2;
        }
    }
    if (IS_UTF8_3BYTE(*start)) {
        if (start + 2 >= end) { // 检测到3字节编码头时，后面会有两个字节的位置
            return -1;
        }
        if (IS_UTF8_EXTRA(*(start + 1)) && IS_UTF8_EXTRA(*(start + 2))) {
            return 3;
        }
    }
    if (IS_UTF8_4BYTE(*start)) {
        if (start + 3 >= end) {
            return -1;
        }
        if (IS_UTF8_EXTRA(*(start + 1)) && IS_UTF8_EXTRA(*(start + 2)) && IS_UTF8_EXTRA(*(start + 3))) {
            return 4;
        }
    }

    return -1;
}

iconv_t convert_open(const char *fromCode, const char *toCode)
{
    iconv_t iconv = nullptr;
    iconv = iconv_open(toCode, fromCode);
    if (iconv == nullptr) {
        perror("iconv_open error");
    }
    return iconv;
}

void convert_close(iconv_t iconv)
{
    iconv_close(iconv);
}

int convert_code(iconv_t cd, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
    char **pin = &inbuf;
    char **pout = &outbuf;

    memset(outbuf, 0, outlen);
    if (iconv(cd, pin, &inlen, pout, &outlen) == (size_t)-1) {
        return -1;
    }
    return 0;
}

#define CODE_TYPE_UTF8  "utf-8"
#define CODE_TYPE_GBK   "gbk"

bool GBK2UTF8(std::string &out, const std::string &in)
{
    UNUSED(out);
    UNUSED(in);
    iconv_t iconv = convert_open(CODE_TYPE_GBK, CODE_TYPE_UTF8);
    if (iconv == nullptr) {
        return false;
    }

    bool ret = false;

    convert_close(iconv);
    return ret;
}

bool UTF82GBK(std::string &out, const std::string &in)
{
    iconv_t iconv = convert_open(CODE_TYPE_GBK, CODE_TYPE_UTF8);
    if (iconv == nullptr) {
        return false;
    }
    
    std::string temp;
    bool ret = false;
    char pout[32] = {0};
    size_t outlen = sizeof(pout);

    const char *start = in.c_str();
    const char *end = start + in.length();
    while (start < end) {
        int bytes = getChar(start, end - start);
        if (bytes < 0) {    // 说明不是utf8
            ret = false;
            goto _error;
        } else if (bytes == 0) {
            temp.append(start, 1);
            ++start;
        } else {
            if (convert_code(iconv, (char *)start, bytes, pout, outlen) < 0) {
                ret = false;
                goto _error;
            }
            start += bytes;
            temp.append(pout);
        }

        memset(pout, 0, outlen);
    }
    ret = true;

_error:
    convert_close(iconv);
    if (ret) {
        out.append(temp);
    }
    return ret;
}