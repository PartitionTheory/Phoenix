#ifndef ABR_RECONSTRUCT_H
#define ABR_RECONSTRUCT_H

#include <stddef.h>
#include <stdint.h>
#include "bigint.h"
#include "bigint_list.h"
#include "bitwindow.h"
#include "abr_config.h"

/*
 * Reconstruction API (Phoenix Edition)
 *
 * This header defines the accumulator used to rebuild a BigInt
 * from a sequence of windows.
 *
 * Math / Engineering view:
 *   Given windows:
 *
 *       W = [W_0, W_1, ..., W_{N-1}]
 *
 *   each representing w bits (except possibly the last),
 *   reconstruction computes:
 *
 *       x = Σ_{i=0}^{N-1} (value(W_i) << (i * w))
 *
 *   This is the exact inverse of window extraction.
 */

/*
 * ReconstructAcc
 *
 * Holds the intermediate state for reconstruction.
 *
 * Math view:
 *   A is the accumulator for:
 *
 *       x = Σ W_i * 2^(i*w)
 *
 *   It stores:
 *     - window_bits: w
 *     - current BigInt value: x_partial
 */
typedef struct {
    BigInt  acc;          /* accumulator BigInt */
    size_t  window_bits;  /* geometry parameter */
} ReconstructAcc;

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 *
 * Initializes the accumulator with:
 *   acc = 0
 *   window_bits = cfg.bitwidth.window_bits
 *
 * Math view:
 *   A := 0.
 */
void reconstruct_init(ReconstructAcc *r, const AbrConfig *cfg);

/* -----------------------------------------------------------
 * Consume one window
 * -----------------------------------------------------------
 *
 * Adds:
 *
 *   acc := acc + (value(W) << (index * window_bits))
 *
 * Math view:
 *   A := A + W * 2^(i*w)
 *
 * Implementation detail:
 *   - Performs BigInt left-shift
 *   - Performs BigInt addition
 */
int reconstruct_consume(ReconstructAcc *r,
                        const BitWindow *w,
                        size_t index);

/* -----------------------------------------------------------
 * Finalize
 * -----------------------------------------------------------
 *
 * Writes the final BigInt into *out.
 *
 * Math view:
 *   out := A.
 */
int reconstruct_finalize(ReconstructAcc *r, BigInt *out);

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 *
 * Frees the accumulator BigInt.
 *
 * Math view:
 *   Releases A.
 */
void reconstruct_free(ReconstructAcc *r);

#endif /* ABR_RECONSTRUCT_H */
