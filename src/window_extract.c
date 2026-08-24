#include <stdlib.h>
#include "window_extract.h"
#include "bitwindow.h"
#include "bigint_list.h"
#include "bitwidth.h"

/*
 * Window Extraction (Phoenix Edition)
 *
 * Implements:
 *   window_extract_all()
 *   window_extract_one()
 *
 * Math / Engineering view:
 *   Given BigInt x with bitstring B and window size w:
 *
 *       N = ceil(|B| / w)
 *
 *   Windows:
 *
 *       W_i = B[i*w : (i+1)*w]
 *
 *   subject to geometry constraints:
 *       |B| <= max_bits
 *       N   <= max_windows
 */

/* -----------------------------------------------------------
 * Extract a single window
 * -----------------------------------------------------------
 */
int window_extract_one(const BigInt *src,
                       const AbrConfig *cfg,
                       size_t index,
                       BitWindow *out)
{
    size_t wbits = cfg->bitwidth.window_bits;

    if (src->bit_len == 0) {
        bitwindow_init(out, wbits);
        return 0;
    }

    size_t N = bitwidth_window_count(src->bit_len, &cfg->bitwidth);
    if (index >= N)
        return -1;

    size_t start_bit = index * wbits;

    bitwindow_init(out, wbits);
    return bitwindow_set_from_bigint(out, src, start_bit);
}

/* -----------------------------------------------------------
 * Extract all windows
 * -----------------------------------------------------------
 */
int window_extract_all(const BigInt *src,
                       const AbrConfig *cfg,
                       BigIntList *out)
{
    /* Validate bit-length */
    if (!bitwidth_validate(src->bit_len, &cfg->bitwidth))
        return -1;

    size_t N = bitwidth_window_count(src->bit_len, &cfg->bitwidth);

    if (N > cfg->max_windows)
        return -2;

    /* Clear output list */
    bigint_list_clear(out);

    /* Extract each window */
    for (size_t i = 0; i < N; i++) {
        BitWindow w;
        bitwindow_init(&w, cfg->bitwidth.window_bits);

        if (window_extract_one(src, cfg, i, &w) != 0) {
            bitwindow_free(&w);
            return -3;
        }

        /* Append BigInt value of window */
        if (bigint_list_append(out, &w.value) != 0) {
            bitwindow_free(&w);
            return -4;
        }

        bitwindow_free(&w);
    }

    return 0;
}
