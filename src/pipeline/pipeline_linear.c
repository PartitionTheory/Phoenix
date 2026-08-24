#include "pipeline.h"
#include "bigint_list.h"
#include "bitwindow.h"

/*
 * Pipeline Linear (Phoenix Edition)
 *
 * Implements:
 *   pipeline_linear()
 *
 * Math / Engineering view:
 *   Linear pipeline is the identity transform:
 *
 *       P(W) = W
 *
 *   This mode is used for:
 *     - baseline correctness
 *     - round-trip validation
 *     - plugin testing
 *     - VM integration
 */

/* -----------------------------------------------------------
 * Linear pipeline
 * -----------------------------------------------------------
 *
 * Input:
 *   windows_in  — BigIntList of windows
 *
 * Output:
 *   windows_out — BigIntList of windows (same as input)
 *
 * Behavior:
 *   Deep-copy each window.
 */
int pipeline_linear(const BigIntList *windows_in,
                    BigIntList *windows_out)
{
    bigint_list_clear(windows_out);

    for (size_t i = 0; i < windows_in->count; i++) {
        const BigInt *src = &windows_in->items[i];

        if (bigint_list_append(windows_out, src) != 0)
            return -1;
    }

    return 0;
}
