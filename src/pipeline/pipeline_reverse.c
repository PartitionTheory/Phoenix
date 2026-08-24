#include "pipeline.h"
#include "bigint_list.h"

/*
 * Pipeline Reverse (Phoenix Edition)
 *
 * Implements:
 *   pipeline_reverse()
 *
 * Math / Engineering view:
 *   Reverse pipeline transforms:
 *
 *       [W_0, W_1, ..., W_{N-1}]
 *
 *   into:
 *
 *       [W_{N-1}, ..., W_1, W_0]
 *
 *   This mode is used for:
 *     - reverse system run mode
 *     - VM instruction testing
 *     - plugin validation
 *     - domain invariants
 */

/* -----------------------------------------------------------
 * Reverse pipeline
 * -----------------------------------------------------------
 *
 * Input:
 *   windows_in  — BigIntList of windows
 *
 * Output:
 *   windows_out — BigIntList of windows in reverse order
 *
 * Behavior:
 *   Deep-copy each window in reverse index order.
 */
int pipeline_reverse(const BigIntList *windows_in,
                     BigIntList *windows_out)
{
    bigint_list_clear(windows_out);

    if (windows_in->count == 0)
        return 0;

    for (size_t i = windows_in->count; i-- > 0;) {
        const BigInt *src = &windows_in->items[i];

        if (bigint_list_append(windows_out, src) != 0)
            return -1;
    }

    return 0;
}
