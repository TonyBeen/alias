/*************************************************************************
    > File Name: code_convert.cpp
    > Author: hsz
    > Brief:
    > Created Time: 2024年07月19日 星期五 16时44分28秒
 ************************************************************************/

#include "utils/code_convert.h"

#include <string.h>

#include <iconv.h>

#include "utils/exception.h"
#include "utils/errors.h"

#define CODE_UNSUPPORT "UNSUPPORT"

#define INVALID_ICONV_HANDLE ((iconv_t)-1)
#define INVALID_ICONV_RETURN ((size_t)-1)

#define CACHE_SIZE  1024

namespace eular {
CodeConvert::CodeConvert() :
    m_codeConvHandle(INVALID_ICONV_HANDLE),
    m_cacheSize(0)
{
}

bool CodeConvert::convertBegin(CodeFlag from, CodeFlag to)
{
    iconv_t cd = iconv_open(_flag2str(to), _flag2str(from));
    if (cd == INVALID_ICONV_HANDLE) {
        perror("iconv_open");
        return false;
    }

    m_codeFrom = from;
    m_codeTo = to;
    m_codeConvHandle = cd;
    return true;
}

bool CodeConvert::convert(const std::string &from, std::string &to)
{
    if (from.empty()) {
        return false;
    }

    char *pBegin = const_cast<char *>(from.c_str());
    size_t fromSize = from.size();
    std::string output;
    output.reserve(_computeOutSize(m_codeFrom, m_codeTo, from.size()));
    char outputBuf[CACHE_SIZE] = {0};

    bool result = true;
    do {
        char *pOutputBuf = outputBuf;
        size_t leftOutputLen = CACHE_SIZE;

        size_t nRet = iconv(m_codeConvHandle, &pBegin, &fromSize, &pOutputBuf, &leftOutputLen);
        if (INVALID_ICONV_RETURN == nRet) {
            switch (errno) {
            case EINVAL:
                printf("An incomplete multibyte sequence has been encountered in the input.\n");
                result = false;
                break;
            case EILSEQ:
                printf("An invalid multibyte sequence has been encountered in the input.\n");
                result = false;
                break;
            case E2BIG:
                break;
            default:
                throw std::runtime_error("unknown error");
            }
        }

        if (!result) {
            output.clear();
            break;
        }

        // printf("nRet: %zu left: %zu leftOutputLen: %zu\n", nRet, fromSize, leftOutputLen);
        output.append(outputBuf, CACHE_SIZE - leftOutputLen);
    } while (fromSize > 0);

    to.append(output);
    return result;
}

void CodeConvert::convertEnd()
{
    if (m_codeConvHandle != INVALID_ICONV_HANDLE) {
        iconv_close(m_codeConvHandle);
    }
}

int32_t CodeConvert::UTF8ToGBK(const std::string &u8String, std::string &gbkString)
{
    if (u8String.empty()) {
        return Status::INVALID_PARAM;
    }

    iconv_t iconvHandle = iconv_open("GBK", "UTF-8");
    if (iconvHandle == INVALID_ICONV_HANDLE) {
        perror("iconv_open");
        return -errno;
    }

    char *pU8Begin = (char *)u8String.c_str();
    size_t inputSize = u8String.size();

    std::string gbkResult;
    gbkResult.reserve(_computeOutSize(UTF8, GBK, u8String.size()));

    bool result = true;
    do {
        char outputBuf[CACHE_SIZE] = {0};
        char *pOutputBuf = outputBuf;

        size_t outputLen = CACHE_SIZE;
        size_t leftOutputLen = CACHE_SIZE;

        size_t nRet = iconv(iconvHandle, &pU8Begin, &inputSize, &pOutputBuf, &leftOutputLen);
        if (INVALID_ICONV_RETURN == nRet) {
            switch (errno) {
            case EINVAL:
                printf("An incomplete multibyte sequence has been encountered in the input.\n");
                result = false;
                break;
            case EILSEQ:
                printf("An invalid multibyte sequence has been encountered in the input.\n");
                result = false;
                break;
            case E2BIG:
                break;
            default:
                throw std::runtime_error("unknown error");
            }
        }

        if (!result) {
            gbkResult.clear();
            break;
        }

        // printf("nRet: %zu left: %zu leftOutputLen: %zu\n", nRet, inputSize, leftOutputLen);
        gbkResult.append(outputBuf, (outputLen - leftOutputLen));
    } while (inputSize > 0);

    gbkString.append(gbkResult);
    iconv_close(iconvHandle);
    return Status::OK;
}

int32_t CodeConvert::GBKToUTF8(const std::string &gbkString, std::string &u8String)
{
    if (gbkString.empty()) {
        return Status::INVALID_PARAM;
    }

    iconv_t iconvHandle = iconv_open("UTF-8", "GBK");
    if (iconvHandle == INVALID_ICONV_HANDLE) {
        perror("iconv_open");
        return -errno;
    }

    char *pBegin = (char *)gbkString.c_str();
    size_t inputSize = gbkString.size();

    std::string u8Result;
    u8Result.reserve(_computeOutSize(GBK, UTF8, gbkString.size()));

    bool result = true;
    do {
        char outputBuf[CACHE_SIZE] = {0};
        char *pOutputBuf = outputBuf;

        size_t outputLen = CACHE_SIZE;
        size_t leftOutputLen = CACHE_SIZE;

        size_t nRet = iconv(iconvHandle, &pBegin, &inputSize, &pOutputBuf, &leftOutputLen);
        if (INVALID_ICONV_RETURN == nRet) {
            switch (errno) {
            case EINVAL:
                printf("An incomplete multibyte sequence has been encountered in the input.\n");
                result = false;
                break;
            case EILSEQ:
                printf("An invalid multibyte sequence has been encountered in the input.\n");
                result = false;
                break;
            case E2BIG:
                break;
            default:
                throw std::runtime_error("unknown error");
            }
        }

        if (!result) {
            u8Result.clear();
            break;
        }

        // printf("nRet: %zu left: %zu leftOutputLen: %zu\n", nRet, inputSize, leftOutputLen);
        u8Result.append(outputBuf, (outputLen - leftOutputLen));
    } while (inputSize > 0);

    u8String.append(u8Result);
    iconv_close(iconvHandle);
    return Status::OK;
}

const char *CodeConvert::_flag2str(CodeFlag flag)
{
    const char *str = CODE_UNSUPPORT;
    switch (flag) {
    case CodeFlag::GBK:
        str = "GBK";
        break;
    case CodeFlag::GB2312:
        str = "GB2312";
        break;
    case CodeFlag::UTF8:
        str = "UTF-8";
        break;
    case CodeFlag::UTF16LE:
        str = "UTF-16LE";
        break;
    case CodeFlag::UTF16BE:
        str = "UTF-16BE";
        break;
    default:
        break;
    }

    return str;
}

uint32_t CodeConvert::_computeOutSize(CodeFlag from, CodeFlag to, uint32_t inputSize)
{
    if (from == to) {
        return inputSize;
    }

    switch (to) {
    // 两个字节编码
    case CodeFlag::GBK:
    case CodeFlag::GB2312:
    case CodeFlag::UTF16LE:
    case CodeFlag::UTF16BE:
        return _computeOutSizeToGBK_UTF16(from, to, inputSize);
    case CodeFlag::UTF8:
        return _computeOutSizeToUTF8(from, to, inputSize);
    default:
        break;
    }

    return 0;
}

uint32_t CodeConvert::_computeOutSizeToGBK_UTF16(CodeFlag from, CodeFlag to, uint32_t inputSize)
{
    switch (from) {
    case CodeFlag::UTF8:
        return 2 * inputSize; // 按照最大size计算
    default:
        return inputSize;
    }

    return 0;
}

uint32_t CodeConvert::_computeOutSizeToUTF8(CodeFlag from, CodeFlag to, uint32_t inputSize)
{
    return 2 * inputSize; // 按照最大size计算, 4 * inputSize / 2
}

} // namespace eular
