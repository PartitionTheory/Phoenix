#ifndef ABR_BIGINT_H
#define ABR_BIGINT_H

#include <stddef.h>
#include <stdint.h>

/*
 * BigInt (Phoenix Edition)
 *
 * This is the canonical arbitrary-precision integer type used
 * throughout the ABR system. It is intentionally simple:
 *
 *   - bytes:     little-endian byte array
 *   - byte_len:  number of bytes
 *   - bit_len:   number of bits (<= 8 * byte_len)
 *
 * Math / Engineering view:
 *   A BigInt represents a non-negative integer:
 *
 *       x = Σ_{i=0}^{byte_len-1} bytes[i] * 256^i
 *
 *   bit_len is the length of the binary representation of x.
 *
 *   The representation is canonical:
 *     - No leading zero bytes
 *     - bit_len = floor(log2(x)) + 1, except x = 0 → bit_len = 0
 */
typedef struct {
    uint8_t *bytes;     /* little-endian byte array */
    size_t   byte_len;  /* number of bytes */
    size_t   bit_len;   /* number of bits */
} BigInt;

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 *
 * Initializes a BigInt with zero value.
 *
 * Math view:
 *   x = 0 → bytes = empty, byte_len = 0, bit_len = 0.
 */
void bigint_init(BigInt *bi);

/* -----------------------------------------------------------
 * Allocation
 * -----------------------------------------------------------
 *
 * Allocates a BigInt with a given number of bytes.
 *
 * Math view:
 *   Allocates space for representing integers up to:
 *
 *       2^(8 * byte_len) - 1
 */
int bigint_alloc(BigInt *bi, size_t byte_len);

/* -----------------------------------------------------------
 * Set from raw bytes
 * -----------------------------------------------------------
 *
 * Sets the BigInt from a little-endian byte array.
 *
 * Math view:
 *   x = Σ bytes[i] * 256^i
 *
 *   bit_len is computed as:
 *
 *       bit_len = 8 * (byte_len - 1) + floor(log2(bytes[last])) + 1
 */
int bigint_set(BigInt *bi, const uint8_t *bytes, size_t byte_len);

/* -----------------------------------------------------------
 * Copy
 * -----------------------------------------------------------
 *
 * Copies one BigInt into another.
 *
 * Math view:
 *   y := x
 */
int bigint_copy(BigInt *dst, const BigInt *src);

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 *
 * Frees the internal byte buffer.
 *
 * Math view:
 *   Releases the representation of x.
 */
void bigint_free(BigInt *bi);

#endif /* ABR_BIGINT_H */
