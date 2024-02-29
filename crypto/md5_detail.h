#ifndef __MD5_DETAIL_H__
#define __MD5_DETAIL_H__

#include <stdint.h>
#include <string>

#define DIGEST_SIZE 16

class MD5
{
public:
    /* Construct a MD5 object with a string. */
    MD5(const std::string &message);

    MD5(const void *msg, uint32_t msgLen);

    /* Generate md5 digest. */
    const uint8_t *getDigest();

    /* Convert digest to string value */
    std::string to_string();

    static void GetDigest(const std::string &message, uint8_t *output);

    static void GetDigest(const void *msg, uint32_t msgLen, uint8_t *output);

private:
    /* Initialization the md5 object, processing another message block,
     * and updating the context.*/
    void init(const uint8_t *input, size_t len);

    /* MD5 basic transformation. Transforms state based on block. */
    void transform(const uint8_t block[64]);

    /* Encodes input (usigned long) into output (uint8_t). */
    void encode(const uint32_t *input, uint8_t *output, size_t length);

    /* Decodes input (uint8_t) into output (usigned long). */
    void decode(const uint8_t *input, uint32_t *output, size_t length);

private:
    /* Flag for mark whether calculate finished. */
    bool mFinished;

    /* state (ABCD). */
    uint32_t mState[4];

    /* number of bits, low-order word first. */
    uint32_t mCount[2];

    /* input buffer. */
    uint8_t mBuffer[64];

    /* message digest. */
    uint8_t mDigest[DIGEST_SIZE];
};

#endif // __MD5_DETAIL_H__