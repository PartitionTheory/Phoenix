#include "pipeline.h"
#include "bigint_list.h"
#include "bigint.h"

/*
 * Pipeline Init (Phoenix Edition)
 *
 * Implements:
 *   pipeline_init()
 *
 * Math / Engineering view:
 *   Initialization pipeline prepends a special window W_init:
 *
 *       P(W) = [W_init, W_0, W_1, ..., W_{N-1}]
 *
 *   W_init is defined as:
 *
 *       W_init = BigInt(1)
 *
 *   This mode is used for:
 *     - system initialization mode
 *     - VM initialization semantics
 *     - plugin initialization behavior
 *     - domain invariants (init symmetry)
 */

/* -----------------------------------------------------------
 * Create initialization window
 * -----------------------------------------------------------
 */
static int create_init_window(BigInt *out)
{
    /* W_init = BigInt(1) */
    uint8_t b = 1;

    if (bigint_alloc(out, 1) != 0)
        return -1;

    out->bytes[0] = b;
    out->byte_len = 1;
    out->bit_len  = 1;

    return 0;
}

/* -----------------------------------------------------------
 * Initialization pipeline
 * -----------------------------------------------------------
 *
 * Input:
 *   windows_in  — BigIntList of windows
 *
 * Output:
 *   windows_out — BigIntList with W_init prepended
 *
 * Behavior:
 *   1. Create W_init
 *   2. Append W_init
 *   3. Append all windows_in
 */
int pipeline_init(const BigIntList *windows_in,
                  BigIntList *windows_out)
{
    bigint_list_clear(windows_out);

    /* 1. Create initialization window */
    BigInt init;
    bigint_init(&init);

    if (create_init_window(&init) != 0)
        return -1;

    /* 2. Append W_init */
    if (bigint_list_append(windows_out, &init) != 0) {
        bigint_free(&init);
        return -2;
    }

    bigint_free(&init);

    /* 3. Append all input windows */
    for (size_t i = 0; i < windows_in->count; i++) {
        const BigInt *src = &windows_in->items[i];

        if (bigint_list_append(windows_out, src) != 0)
            return -3;
    }

    return 0;
}
