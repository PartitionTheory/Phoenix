#ifndef ABR_CONFIG_H
#define ABR_CONFIG_H

#include <stddef.h>
#include <stdint.h>
#include "bitwidth.h"

/*
 * ABR Phoenix Configuration
 *
 * This header defines the global configuration structures that control:
 *  - bitwidth/window sizing for extraction
 *  - limits on stream length and window counts
 *  - knobs that the CLI, pipeline, VM, and system share.
 *
 * Math / Engineering view:
 *  - We treat the bitwidth configuration as a small parameter vector
 *    controlling the mapping from a BigInt stream to fixed-size windows.
 *  - AbrConfig is the outer configuration object that can be passed
 *    through the entire system without needing to know internal details.
 */

/*
 * bitwidth_config
 *
 * This structure captures the "geometry" of the windowing process:
 *
 *  - window_bits:
 *      The number of bits per window. For example, 8, 16, 32, 64.
 *      This is the fundamental unit size for window_extract_all().
 *
 *  - max_bits:
 *      A global cap on the total number of bits that may be processed
 *      in a single run. This prevents pathological inputs from
 *      exhausting memory or time.
 *
 * From a math perspective:
 *  - We are partitioning a bitstring B of length |B| into chunks of
 *    size window_bits, with a guard that |B| <= max_bits.
 *  - window_extract_all() will effectively compute:
 *        ceil(|B| / window_bits)
 *    windows, subject to the max_bits constraint.
 */

/*
 * AbrConfig
 *
 * This is the top-level configuration object for the ABR system.
 *
 * It currently embeds:
 *  - bitwidth: the bitwidth_config described above.
 *
 * It is intentionally small and opaque to most callers; they only
 * need to know that AbrConfig can be passed to:
 *  - window_extract_all()
 *  - abr_stream
 *  - pipeline / VM / system entry points
 *
 * Math / Engineering view:
 *  - AbrConfig is a parameter bundle.
 *  - It can be extended later with:
 *      * max_windows
 *      * flags for reverse/branching runs
 *      * plugin-related limits
 *    without breaking existing code that only cares about bitwidth.
 */
typedef struct {
    bitwidth_config bitwidth;
    size_t max_windows;
    uint32_t flags;
} AbrConfig;

#endif /* ABR_CONFIG_H */
