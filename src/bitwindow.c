#include <stdlib.h>
#include <string.h>
#include "bitwindow.h"
#include "bigint.h"

/*
 * BitWindow (Phoenix Edition)
 *
 * Implements:
 *   bitwindow_init()
 *   bitwindow_set_from_bigint()
 *   bitwindow_free()
 *
 * Math / Engineering view:
 *   A BitWindow represents:
 *
 *       W = B[start : start + w]
 *
 *   where B is the bitstring of a BigInt.
 */

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 */
void bitwindow_init(BitWindow *w, size_t window_bits)
{
    bigint_init(&w->value);
    w->window_bits = window_bits;
}

/* -----------------------------------------------------------
 * Extract bits from BigInt
 * -----------------------------------------------------------
 *
 * Extracts:
 *   W = B[start_bit : start_bit + window_bits]
 *
 * Implementation:
 *   - Compute byte/bit offsets
 *   - Copy relevant bytes
 *   - Shift right to align
 *   - Mask off extra bits
 */
int bitwindow_set_from_bigint(BitWindow *w,
                              const BigInt *src,
                              size_t start_bit)
{
    size_t wbits = w->window_bits;

    if (src->bit_len == 0) {
        /* Zero window */
        bigint_init(&w->value);
        return 0;
    }

    /* Compute start byte and bit offset */
    size_t start_byte = start_bit / 8;
    size_t bit_offset = start_bit % 8;

    /* Compute number of bits available */
    if (start_bit >= src->bit_len)
        return -1;

    size_t end_bit = start_bit + wbits;
    if (end_bit > src->bit_len)
        end_bit = src->bit_len;

    size_t slice_bits = end_bit - start_bit;
    size_t slice_bytes = (slice_bits + 7) / 8;

    /* Allocate window BigInt */
    if (bigint_alloc(&w->value, slice_bytes) != 0)
        return -2;

    /* Copy raw bytes */
    for (size_t i = 0; i < slice_bytes; i++) {
        size_t src_index = start_byte + i;
        if (src_index < src->byte_len)
            w->value.bytes[i] = src->bytes[src_index];
        else
            w->value.bytes[i] = 0;
    }

    /* Right-shift by bit_offset */
    if (bit_offset > 0) {
        uint8_t carry = 0;
        for (size_t i = slice_bytes; i-- > 0;) {
            uint8_t new_carry = w->value.bytes[i] << (8 - bit_offset);
            w->value.bytes[i] = (w->value.bytes[i] >> bit_offset) | carry;
            carry = new_carry;
        }
    }

    /* Mask off extra bits in final byte */
    size_t valid_bits_last = slice_bits % 8;
    if (valid_bits_last != 0) {
        uint8_t mask = (1u << valid_bits_last) - 1u;
        w->value.bytes[slice_bytes - 1] &= mask;
    }

    /* Compute bit_len */
    uint8_t last = w->value.bytes[slice_bytes - 1];
    size_t bits = 0;
    while (last > 0) {
        last >>= 1;
        bits++;
    }
    w->value.bit_len = (slice_bytes - 1) * 8 + bits;

    return 0;
}

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 */
void bitwindow_free(BitWindow *w)
{
    bigint_free(&w->value);
    w->window_bits = 0;
}
