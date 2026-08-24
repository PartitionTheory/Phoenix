#ifndef ABR_BITWINDOW_H
#define ABR_BITWINDOW_H

#include <stddef.h>
#include <stdint.h>
#include "bigint.h"

/*
 * BitWindow (Phoenix Edition)
 *
 * A BitWindow is a fixed-size slice of a BigInt's bitstring.
 *
 * Math / Engineering view:
 *   Given a BigInt x with bitstring B and window size w,
 *   the i-th window is:
 *
 *       W_i = B[i*w : (i+1)*w]
 *
 *   represented as a BigInt.
 *
 *   If the last window has fewer than w bits, it is padded
 *   naturally by the BigInt representation (no explicit zeros).
 *
 * This header defines:
 *   - BitWindow struct
 *   - initialization
 *   - setting from a BigInt slice
 *   - freeing
 */

/*
 * BitWindow
 *
 * Represents a window of exactly window_bits bits (except possibly
 * the last window, which may contain fewer bits).
 *
 * Math view:
 *   W_i is a BigInt representing the integer value of the slice.
 */
typedef struct {
    BigInt value;        /* BigInt representation of the window */
    size_t window_bits;  /* number of bits this window represents */
} BitWindow;

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 *
 * Initializes the window with zero value.
 *
 * Math view:
 *   W := 0.
 */
void bitwindow_init(BitWindow *w, size_t window_bits);

/* -----------------------------------------------------------
 * Set from BigInt slice
 * -----------------------------------------------------------
 *
 * Extracts bits [start_bit, start_bit + window_bits) from src
 * and stores them in w->value.
 *
 * Math view:
 *   W := B[start_bit : start_bit + window_bits]
 *
 * Implementation detail:
 *   - Extracts bits using BigInt arithmetic
 *   - Handles partial final windows
 */
int bitwindow_set_from_bigint(BitWindow *w,
                              const BigInt *src,
                              size_t start_bit);

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 *
 * Frees the internal BigInt.
 *
 * Math view:
 *   Releases W.
 */
void bitwindow_free(BitWindow *w);

#endif /* ABR_BITWINDOW_H */
