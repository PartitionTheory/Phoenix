#ifndef ABR_WINDOW_H
#define ABR_WINDOW_H

#include <stddef.h>
#include <stdint.h>
#include "bigint.h"
#include "bitwindow.h"

/*
 * Window Descriptor (Phoenix Edition)
 *
 * This header defines the canonical window descriptor used by:
 *   - pipeline stages
 *   - VM instructions
 *   - plugin interfaces
 *   - reconstruction
 *
 * Math / Engineering view:
 *   A Window is a thin wrapper around BitWindow that exposes
 *   only the stable ABI fields needed by higher-level systems.
 *
 *   BitWindow contains:
 *       BigInt value
 *       size_t window_bits
 *
 *   Window simply aliases this structure for clarity and
 *   subsystem independence.
 */

/*
 * Window
 *
 * A stable alias for BitWindow.
 *
 * Math view:
 *   Window = BitWindow = (value, window_bits)
 *
 * Rationale:
 *   - BitWindow is the geometry-aware representation.
 *   - Window is the subsystem-neutral representation.
 *   - They are identical at the binary level.
 */
typedef BitWindow Window;

/* -----------------------------------------------------------
 * Initialize a window
 * -----------------------------------------------------------
 *
 * Math view:
 *   W := 0 with geometry parameter w.
 */
static inline void window_init(Window *w, size_t window_bits)
{
    bitwindow_init(w, window_bits);
}

/* -----------------------------------------------------------
 * Set window from BigInt slice
 * -----------------------------------------------------------
 *
 * Math view:
 *   W := B[start_bit : start_bit + window_bits]
 */
static inline int window_set_from_bigint(Window *w,
                                         const BigInt *src,
                                         size_t start_bit)
{
    return bitwindow_set_from_bigint(w, src, start_bit);
}

/* -----------------------------------------------------------
 * Free window
 * -----------------------------------------------------------
 *
 * Math view:
 *   Releases W.
 */
static inline void window_free(Window *w)
{
    bitwindow_free(w);
}

#endif /* ABR_WINDOW_H */
