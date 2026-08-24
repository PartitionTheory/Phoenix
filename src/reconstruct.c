#include <stdlib.h>
#include <string.h>
#include "reconstruct.h"
#include "bigint.h"

/*
 * Reconstruction (Phoenix Edition)
 *
 * Implements:
 *   reconstruct_init()
 *   reconstruct_consume()
 *   reconstruct_finalize()
 *   reconstruct_free()
 *
 * Math / Engineering view:
 *   Given windows W_i representing w bits each:
 *
 *       x = Σ W_i * 2^(i*w)
 *
 *   This file implements the accumulator A that builds x.
 */

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 */
void reconstruct_init(ReconstructAcc *r, const AbrConfig *cfg)
{
    bigint_init(&r->acc);
    r->window_bits = cfg->bitwidth.window_bits;
}

/* -----------------------------------------------------------
 * Left-shift BigInt by k bits
 * -----------------------------------------------------------
 *
 * Internal helper:
 *   y = x << k
 *
 * Math:
 *   Multiply by 2^k.
 */
static int bigint_shift_left(BigInt *dst, const BigInt *src, size_t k)
{
    size_t byte_shift = k / 8;
    size_t bit_shift  = k % 8;

    /* Allocate new BigInt */
    size_t new_bytes = src->byte_len + byte_shift + 1; /* +1 for carry */
    if (bigint_alloc(dst, new_bytes) != 0)
        return -1;

    memset(dst->bytes, 0, new_bytes);

    /* Copy with byte shift */
    for (size_t i = 0; i < src->byte_len; i++)
        dst->bytes[i + byte_shift] = src->bytes[i];

    /* Bit shift */
    if (bit_shift > 0) {
        uint8_t carry = 0;
        for (size_t i = byte_shift; i < new_bytes; i++) {
            uint8_t new_carry = dst->bytes[i] >> (8 - bit_shift);
            dst->bytes[i] = (dst->bytes[i] << bit_shift) | carry;
            carry = new_carry;
        }
    }

    /* Compute bit_len */
    size_t i = new_bytes;
    while (i > 0 && dst->bytes[i - 1] == 0)
        i--;

    if (i == 0) {
        dst->bit_len = 0;
        dst->byte_len = 0;
        return 0;
    }

    dst->byte_len = i;

    uint8_t last = dst->bytes[i - 1];
    size_t bits = 0;
    while (last > 0) {
        last >>= 1;
        bits++;
    }

    dst->bit_len = (i - 1) * 8 + bits;
    return 0;
}

/* -----------------------------------------------------------
 * Add two BigInts: dst = a + b
 * -----------------------------------------------------------
 */
static int bigint_add(BigInt *dst, const BigInt *a, const BigInt *b)
{
    size_t max = (a->byte_len > b->byte_len) ? a->byte_len : b->byte_len;
    if (bigint_alloc(dst, max + 1) != 0)
        return -1;

    uint16_t carry = 0;

    for (size_t i = 0; i < max; i++) {
        uint16_t av = (i < a->byte_len) ? a->bytes[i] : 0;
        uint16_t bv = (i < b->byte_len) ? b->bytes[i] : 0;

        uint16_t sum = av + bv + carry;
        dst->bytes[i] = (uint8_t)(sum & 0xFF);
        carry = sum >> 8;
    }

    dst->bytes[max] = (uint8_t)carry;

    /* Compute bit_len */
    size_t i = max + 1;
    while (i > 0 && dst->bytes[i - 1] == 0)
        i--;

    if (i == 0) {
        dst->bit_len = 0;
        dst->byte_len = 0;
        return 0;
    }

    dst->byte_len = i;

    uint8_t last = dst->bytes[i - 1];
    size_t bits = 0;
    while (last > 0) {
        last >>= 1;
        bits++;
    }

    dst->bit_len = (i - 1) * 8 + bits;
    return 0;
}

/* -----------------------------------------------------------
 * Consume one window
 * -----------------------------------------------------------
 *
 * A := A + (W << (index * w))
 */
int reconstruct_consume(ReconstructAcc *r,
                        const BitWindow *w,
                        size_t index)
{
    BigInt shifted;
    bigint_init(&shifted);

    size_t k = index * r->window_bits;

    if (bigint_shift_left(&shifted, &w->value, k) != 0) {
        bigint_free(&shifted);
        return -1;
    }

    BigInt new_acc;
    bigint_init(&new_acc);

    if (bigint_add(&new_acc, &r->acc, &shifted) != 0) {
        bigint_free(&shifted);
        bigint_free(&new_acc);
        return -2;
    }

    bigint_free(&shifted);
    bigint_free(&r->acc);
    r->acc = new_acc;

    return 0;
}

/* -----------------------------------------------------------
 * Finalize
 * -----------------------------------------------------------
 */
int reconstruct_finalize(ReconstructAcc *r, BigInt *out)
{
    return bigint_copy(out, &r->acc);
}

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 */
void reconstruct_free(ReconstructAcc *r)
{
    bigint_free(&r->acc);
    r->window_bits = 0;
}
