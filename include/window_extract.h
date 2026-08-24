#ifndef ABR_WINDOW_EXTRACT_H
#define ABR_WINDOW_EXTRACT_H

#include <stddef.h>
#include <stdint.h>
#include "bigint.h"
#include "bigint_list.h"
#include "bitwindow.h"
#include "bitwidth.h"
#include "abr_config.h"

/*
 * Window Extraction API (Phoenix Edition)
 *
 * This header defines the interface for slicing a BigInt into
 * fixed-size windows according to the geometry defined by
 * bitwidth_config.
 *
 * Math / Engineering view:
 *   Given a BigInt x with bitstring B and window size w,
 *   window_extract_all() computes:
 *
 *       W = [W_0, W_1, ..., W_{N-1}]
 *
 *   where:
 *
 *       W_i = B[i*w : (i+1)*w]
 *
 *   and:
 *
 *       N = ceil(|B| / w)
 *
 *   subject to:
 *
 *       |B| <= cfg.max_bits
 *       N   <= cfg.max_windows
 */

/* -----------------------------------------------------------
 * Extract all windows
 * -----------------------------------------------------------
 *
 * Parameters:
 *   - src: input BigInt
 *   - cfg: geometry configuration
 *   - out: output list of windows (BigIntList)
 *
 * Returns:
 *   0 on success
 *   non-zero on error (invalid geometry, too many windows, etc.)
 *
 * Math view:
 *   Computes the sequence W = [W_0, ..., W_{N-1}].
 */
int window_extract_all(const BigInt *src,
                       const AbrConfig *cfg,
                       BigIntList *out);

/* -----------------------------------------------------------
 * Extract a single window
 * -----------------------------------------------------------
 *
 * Parameters:
 *   - src: input BigInt
 *   - cfg: geometry configuration
 *   - index: window index i
 *   - out: output BitWindow
 *
 * Returns:
 *   0 on success
 *   non-zero on error (index out of range, etc.)
 *
 * Math view:
 *   Computes W_i = B[i*w : (i+1)*w].
 */
int window_extract_one(const BigInt *src,
                       const AbrConfig *cfg,
                       size_t index,
                       BitWindow *out);

#endif /* ABR_WINDOW_EXTRACT_H */
