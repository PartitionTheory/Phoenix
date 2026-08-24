#include <stdlib.h>
#include <string.h>
#include "bigint.h"

/*
 * BigInt (Phoenix Edition)
 *
 * Implements:
 *   bigint_init()
 *   bigint_alloc()
 *   bigint_set()
 *   bigint_copy()
 *   bigint_free()
 *
 * Math / Engineering view:
 *   BigInt represents:
 *
 *       x = Σ bytes[i] * 256^i
 *
 *   with canonical bit-length.
 */

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 */
void bigint_init(BigInt *bi)
{
    bi->bytes = NULL;
    bi->byte_len = 0;
    bi->bit_len = 0;
}

/* -----------------------------------------------------------
 * Allocation
 * -----------------------------------------------------------
 */
int bigint_alloc(BigInt *bi, size_t byte_len)
{
    bi->bytes = (uint8_t *)malloc(byte_len);
    if (!bi->bytes)
        return -1;

    bi->byte_len = byte_len;
    bi->bit_len = 0; /* will be set later */
    memset(bi->bytes, 0, byte_len);
    return 0;
}

/* -----------------------------------------------------------
 * Set from raw bytes
 * -----------------------------------------------------------
 */
int bigint_set(BigInt *bi, const uint8_t *bytes, size_t byte_len)
{
    if (bigint_alloc(bi, byte_len) != 0)
        return -1;

    memcpy(bi->bytes, bytes, byte_len);

    /* Compute bit_len */
    if (byte_len == 0) {
        bi->bit_len = 0;
        return 0;
    }

    uint8_t last = bytes[byte_len - 1];
    if (last == 0) {
        /* Find highest non-zero byte */
        size_t i = byte_len;
        while (i > 0 && bytes[i - 1] == 0)
            i--;

        if (i == 0) {
            bi->bit_len = 0;
            return 0;
        }

        last = bytes[i - 1];
        bi->byte_len = i;
    }

    /* Compute bits in last byte */
    size_t bits = 0;
    while (last > 0) {
        last >>= 1;
        bits++;
    }

    bi->bit_len = (bi->byte_len - 1) * 8 + bits;
    return 0;
}

/* -----------------------------------------------------------
 * Copy
 * -----------------------------------------------------------
 */
int bigint_copy(BigInt *dst, const BigInt *src)
{
    if (bigint_alloc(dst, src->byte_len) != 0)
        return -1;

    memcpy(dst->bytes, src->bytes, src->byte_len);
    dst->byte_len = src->byte_len;
    dst->bit_len  = src->bit_len;
    return 0;
}

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 */
void bigint_free(BigInt *bi)
{
    free(bi->bytes);
    bi->bytes = NULL;
    bi->byte_len = 0;
    bi->bit_len = 0;
}
