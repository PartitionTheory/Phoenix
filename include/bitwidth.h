#ifndef ABR_BITWIDTH_H
#define ABR_BITWIDTH_H

#include <stddef.h>
#include <stdint.h>

/*
 * Bitwidth Geometry (Phoenix Edition)
 *
 * This header defines the mathematical geometry of ABR windows.
 *
 * A window is a fixed-size slice of a BigInt's bitstring.
 *
 * Math / Engineering view:
 *   Given a bitstring B of length |B| and a window size w,
 *   the number of windows is:
 *
 *       N = ceil(|B| / w)
 *
 *   Each window is a BigInt representing w bits (except possibly
 *   the last window, which may contain fewer bits).
 *
 * This header defines:
 *   - bitwidth_config: the geometry parameters
 *   - helper functions for computing window counts
 */

/*
 * bitwidth_config
 *
 * window_bits:
 *   Number of bits per window (e.g., 8, 16, 32, 64).
 *
 * max_bits:
 *   Maximum allowed bit-length of the input BigInt.
 *
 * Math view:
 *   The geometry is the pair (w, M) where:
 *     w = window_bits
 *     M = max_bits
 */
typedef struct {
    size_t window_bits;
    size_t max_bits;
} bitwidth_config;

/* -----------------------------------------------------------
 * Compute number of windows
 * -----------------------------------------------------------
 *
 * Given:
 *   - bit_len: length of bitstring
 *   - cfg: bitwidth_config
 *
 * Returns:
 *   N = ceil(bit_len / cfg.window_bits)
 *
 * Math view:
 *   N = (bit_len + w - 1) / w
 */
static inline size_t bitwidth_window_count(size_t bit_len, const bitwidth_config *cfg)
{
    if (cfg->window_bits == 0)
        return 0;
    return (bit_len + cfg->window_bits - 1) / cfg->window_bits;
}

/* -----------------------------------------------------------
 * Validate bit-length
 * -----------------------------------------------------------
 *
 * Ensures:
 *   bit_len <= cfg.max_bits
 *
 * Math view:
 *   Checks feasibility of B under geometry constraints.
 */
static inline int bitwidth_validate(size_t bit_len, const bitwidth_config *cfg)
{
    return (bit_len <= cfg->max_bits) ? 1 : 0;
}

#endif /* ABR_BITWIDTH_H */
