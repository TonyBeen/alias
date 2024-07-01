/**
 * @file md5.cpp
 * @The implement of md5.
 * @author Jiewei Wei
 * @mail weijieweijerry@163.com
 * @github https://github.com/JieweiWei
 * @data Oct 19 2014
 *
 */

#include "md5.h"
#include <string.h>

/* Parameters of MD5. */
#define s11 7
#define s12 12
#define s13 17
#define s14 22
#define s21 5
#define s22 9
#define s23 14
#define s24 20
#define s31 4
#define s32 11
#define s33 16
#define s34 23
#define s41 6
#define s42 10
#define s43 15
#define s44 21

/**
 * @Basic MD5 functions.
 *
 * @param there uint32_t.
 *
 * @return one uint32_t.
 */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

/**
 * @Rotate Left.
 *
 * @param {num} the raw number.
 *
 * @param {n} rotate left n.
 *
 * @return the number after rotated left.
 */
#define ROTATELEFT(num, n) (((num) << (n)) | ((num) >> (32 - (n))))

/**
 * @Transformations for rounds 1, 2, 3, and 4.
 */
#define FF(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += F((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }
#define GG(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += G((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }
#define HH(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += H((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }
#define II(a, b, c, d, x, s, ac)            \
    {                                       \
        (a) += I((b), (c), (d)) + (x) + ac; \
        (a) = ROTATELEFT((a), (s));         \
        (a) += (b);                         \
    }

const uint8_t PADDING[64] = {0x80};
const char HEX_NUMBERS[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'a', 'b',
    'c', 'd', 'e', 'f'};

/**
 * @Construct a MD5 object with a string.
 *
 * @param {message} the message will be transformed.
 *
 */
MD5::MD5(const std::string &message) :
    mFinished(false)
{
    /* Reset number of bits. */
    mCount[0] = mCount[1] = 0;

    /* Initialization constants. */
    mState[0] = 0x67452301;
    mState[1] = 0xefcdab89;
    mState[2] = 0x98badcfe;
    mState[3] = 0x10325476;

    /* Initialization the object according to message. */
    reset((const uint8_t *)message.c_str(), message.length());
}

MD5::MD5(const void *msg, uint32_t msgLen) :
    mFinished(false)
{
    /* Reset number of bits. */
    mCount[0] = mCount[1] = 0;

    /* Initialization constants. */
    mState[0] = 0x67452301;
    mState[1] = 0xefcdab89;
    mState[2] = 0x98badcfe;
    mState[3] = 0x10325476;

    /* Initialization the object according to message. */
    reset((const uint8_t *)msg, msgLen);
}

/**
 * @Generate md5 digest.
 *
 * @return the message-digest.
 *
 */
const uint8_t *MD5::getDigest()
{
    if (!mFinished)
    {
        mFinished = true;

        uint8_t bits[8];
        uint32_t oldState[4];
        uint32_t oldCount[2];
        uint32_t index, padLen;

        /* Save current state and count. */
        memcpy(oldState, mState, 16);
        memcpy(oldCount, mCount, 8);

        /* Save number of bits */
        encode(mCount, bits, 8);

        /* Pad out to 56 mod 64. */
        index = (uint32_t)((mCount[0] >> 3) & 0x3f);
        padLen = (index < 56) ? (56 - index) : (120 - index);
        reset(PADDING, padLen);

        /* Append length (before padding) */
        reset(bits, 8);

        /* Store state in digest */
        encode(mState, mDigest, 16);

        /* Restore current state and count. */
        memcpy(mState, oldState, 16);
        memcpy(mCount, oldCount, 8);
    }

    return mDigest;
}

/**
 * @Initialization the md5 object, processing another message block,
 * and updating the context.
 *
 * @param {input} the input message.
 *
 * @param {len} the number btye of message.
 *
 */
void MD5::reset(const void *pInput, size_t len)
{
    const uint8_t *input = (const uint8_t *)pInput;
    uint32_t i, index, partLen;

    mFinished = false;

    /* Compute number of bytes mod 64 */
    index = (uint32_t)((mCount[0] >> 3) & 0x3f);

    /* update number of bits */
    if ((mCount[0] += ((uint32_t)len << 3)) < ((uint32_t)len << 3))
    {
        ++mCount[1];
    }
    mCount[1] += ((uint32_t)len >> 29);

    partLen = 64 - index;

    /* transform as many times as possible. */
    if (len >= partLen)
    {
        memcpy(&mBuffer[index], input, partLen);
        transform(mBuffer);

        for (i = partLen; i + 63 < len; i += 64)
        {
            transform(&input[i]);
        }
        index = 0;
    }
    else
    {
        i = 0;
    }

    /* Buffer remaining input */
    memcpy(&mBuffer[index], &input[i], len - i);
}

/**
 * @MD5 basic transformation. Transforms state based on block.
 *
 * @param {block} the message block.
 */
void MD5::transform(const uint8_t block[64])
{

    uint32_t a = mState[0], b = mState[1], c = mState[2], d = mState[3], x[16];

    decode(block, x, 64);

    /* Round 1 */
    FF(a, b, c, d, x[0], s11, 0xd76aa478);
    FF(d, a, b, c, x[1], s12, 0xe8c7b756);
    FF(c, d, a, b, x[2], s13, 0x242070db);
    FF(b, c, d, a, x[3], s14, 0xc1bdceee);
    FF(a, b, c, d, x[4], s11, 0xf57c0faf);
    FF(d, a, b, c, x[5], s12, 0x4787c62a);
    FF(c, d, a, b, x[6], s13, 0xa8304613);
    FF(b, c, d, a, x[7], s14, 0xfd469501);
    FF(a, b, c, d, x[8], s11, 0x698098d8);
    FF(d, a, b, c, x[9], s12, 0x8b44f7af);
    FF(c, d, a, b, x[10], s13, 0xffff5bb1);
    FF(b, c, d, a, x[11], s14, 0x895cd7be);
    FF(a, b, c, d, x[12], s11, 0x6b901122);
    FF(d, a, b, c, x[13], s12, 0xfd987193);
    FF(c, d, a, b, x[14], s13, 0xa679438e);
    FF(b, c, d, a, x[15], s14, 0x49b40821);

    /* Round 2 */
    GG(a, b, c, d, x[1], s21, 0xf61e2562);
    GG(d, a, b, c, x[6], s22, 0xc040b340);
    GG(c, d, a, b, x[11], s23, 0x265e5a51);
    GG(b, c, d, a, x[0], s24, 0xe9b6c7aa);
    GG(a, b, c, d, x[5], s21, 0xd62f105d);
    GG(d, a, b, c, x[10], s22, 0x2441453);
    GG(c, d, a, b, x[15], s23, 0xd8a1e681);
    GG(b, c, d, a, x[4], s24, 0xe7d3fbc8);
    GG(a, b, c, d, x[9], s21, 0x21e1cde6);
    GG(d, a, b, c, x[14], s22, 0xc33707d6);
    GG(c, d, a, b, x[3], s23, 0xf4d50d87);
    GG(b, c, d, a, x[8], s24, 0x455a14ed);
    GG(a, b, c, d, x[13], s21, 0xa9e3e905);
    GG(d, a, b, c, x[2], s22, 0xfcefa3f8);
    GG(c, d, a, b, x[7], s23, 0x676f02d9);
    GG(b, c, d, a, x[12], s24, 0x8d2a4c8a);

    /* Round 3 */
    HH(a, b, c, d, x[5], s31, 0xfffa3942);
    HH(d, a, b, c, x[8], s32, 0x8771f681);
    HH(c, d, a, b, x[11], s33, 0x6d9d6122);
    HH(b, c, d, a, x[14], s34, 0xfde5380c);
    HH(a, b, c, d, x[1], s31, 0xa4beea44);
    HH(d, a, b, c, x[4], s32, 0x4bdecfa9);
    HH(c, d, a, b, x[7], s33, 0xf6bb4b60);
    HH(b, c, d, a, x[10], s34, 0xbebfbc70);
    HH(a, b, c, d, x[13], s31, 0x289b7ec6);
    HH(d, a, b, c, x[0], s32, 0xeaa127fa);
    HH(c, d, a, b, x[3], s33, 0xd4ef3085);
    HH(b, c, d, a, x[6], s34, 0x4881d05);
    HH(a, b, c, d, x[9], s31, 0xd9d4d039);
    HH(d, a, b, c, x[12], s32, 0xe6db99e5);
    HH(c, d, a, b, x[15], s33, 0x1fa27cf8);
    HH(b, c, d, a, x[2], s34, 0xc4ac5665);

    /* Round 4 */
    II(a, b, c, d, x[0], s41, 0xf4292244);
    II(d, a, b, c, x[7], s42, 0x432aff97);
    II(c, d, a, b, x[14], s43, 0xab9423a7);
    II(b, c, d, a, x[5], s44, 0xfc93a039);
    II(a, b, c, d, x[12], s41, 0x655b59c3);
    II(d, a, b, c, x[3], s42, 0x8f0ccc92);
    II(c, d, a, b, x[10], s43, 0xffeff47d);
    II(b, c, d, a, x[1], s44, 0x85845dd1);
    II(a, b, c, d, x[8], s41, 0x6fa87e4f);
    II(d, a, b, c, x[15], s42, 0xfe2ce6e0);
    II(c, d, a, b, x[6], s43, 0xa3014314);
    II(b, c, d, a, x[13], s44, 0x4e0811a1);
    II(a, b, c, d, x[4], s41, 0xf7537e82);
    II(d, a, b, c, x[11], s42, 0xbd3af235);
    II(c, d, a, b, x[2], s43, 0x2ad7d2bb);
    II(b, c, d, a, x[9], s44, 0xeb86d391);

    mState[0] += a;
    mState[1] += b;
    mState[2] += c;
    mState[3] += d;
}

/**
 * @Encodes input (unsigned long) into output (uint8_t).
 *
 * @param {input} usigned long.
 *
 * @param {output} uint8_t.
 *
 * @param {length} the length of input.
 *
 */
void MD5::encode(const uint32_t *input, uint8_t *output, size_t length)
{

    for (size_t i = 0, j = 0; j < length; ++i, j += 4)
    {
        output[j] = (uint8_t)(input[i] & 0xff);
        output[j + 1] = (uint8_t)((input[i] >> 8) & 0xff);
        output[j + 2] = (uint8_t)((input[i] >> 16) & 0xff);
        output[j + 3] = (uint8_t)((input[i] >> 24) & 0xff);
    }
}

/**
 * @Decodes input (uint8_t) into output (usigned long).
 *
 * @param {input} bytes.
 *
 * @param {output} unsigned long.
 *
 * @param {length} the length of input.
 *
 */
void MD5::decode(const uint8_t *input, uint32_t *output, size_t length)
{
    for (size_t i = 0, j = 0; j < length; ++i, j += 4)
    {
        output[i] = ((uint32_t)input[j]) | (((uint32_t)input[j + 1]) << 8) |
                    (((uint32_t)input[j + 2]) << 16) | (((uint32_t)input[j + 3]) << 24);
    }
}

/**
 * @Convert digest to string value.
 *
 * @return the hex string of digest.
 *
 */
std::string MD5::to_string()
{
    const uint8_t *digest_ = getDigest();
    std::string str;
    str.reserve(16 << 1);
    for (size_t i = 0; i < 16; ++i)
    {
        int t = digest_[i];
        int a = t / 16;
        int b = t % 16;
        str.append(1, HEX_NUMBERS[a]);
        str.append(1, HEX_NUMBERS[b]);
    }
    return str;
}

void MD5::GetDigest(const std::string &message, uint8_t output[MD5_DIGEST_SIZE])
{
    MD5 md5(message);
    const uint8_t *ptr = md5.getDigest();
    memcpy(output, ptr, MD5_DIGEST_SIZE);
}

void MD5::GetDigest(const void *msg, uint32_t msgLen, uint8_t output[MD5_DIGEST_SIZE])
{
    MD5 md5(msg, msgLen);
    const uint8_t *ptr = md5.getDigest();
    memcpy(output, ptr, MD5_DIGEST_SIZE);
}