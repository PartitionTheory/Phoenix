#include "pipeline.h"
#include "bigint_list.h"

/*
 * Pipeline Branching (Phoenix Edition)
 *
 * Implements:
 *   pipeline_branching()
 *
 * Math / Engineering view:
 *   Branching pipeline splits windows into two branches:
 *
 *       Left  branch: windows at even indices
 *       Right branch: windows at odd  indices
 *
 *   Then recombines them:
 *
 *       P(W) = [Left_0, Right_0, Left_1, Right_1, ...]
 *
 *   This mode is used for:
 *     - branching system run mode
 *     - VM branching instruction tests
 *     - plugin branching behavior
 *     - domain invariants
 */

/* -----------------------------------------------------------
 * Branching pipeline
 * -----------------------------------------------------------
 *
 * Input:
 *   windows_in  — BigIntList of windows
 *
 * Output:
 *   windows_out — BigIntList of windows in interleaved branch order
 *
 * Behavior:
 *   1. Split windows into left (even indices) and right (odd indices)
 *   2. Interleave them: L0, R0, L1, R1, ...
 *   3. Deep-copy each window
 */
int pipeline_branching(const BigIntList *windows_in,
                       BigIntList *windows_out)
{
    bigint_list_clear(windows_out);

    BigIntList left;
    BigIntList right;

    bigint_list_init(&left);
    bigint_list_init(&right);

    /* Split into branches */
    for (size_t i = 0; i < windows_in->count; i++) {
        const BigInt *src = &windows_in->items[i];

        if (i % 2 == 0) {
            if (bigint_list_append(&left, src) != 0)
                goto fail;
        } else {
            if (bigint_list_append(&right, src) != 0)
                goto fail;
        }
    }

    /* Interleave branches */
    size_t L = left.count;
    size_t R = right.count;
    size_t M = (L > R) ? L : R;

    for (size_t i = 0; i < M; i++) {
        if (i < L) {
            if (bigint_list_append(windows_out, &left.items[i]) != 0)
                goto fail;
        }
        if (i < R) {
            if (bigint_list_append(windows_out, &right.items[i]) != 0)
                goto fail;
        }
    }

    bigint_list_free(&left);
    bigint_list_free(&right);
    return 0;

fail:
    bigint_list_free(&left);
    bigint_list_free(&right);
    return -1;
}
