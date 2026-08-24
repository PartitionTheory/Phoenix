#include "branch_merge_split.h"
#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI implementation: branch, merge, split
 *
 * Legacy behavior preserved:
 * - Branch: duplicate window
 * - Merge: XOR halves
 * - Split: keep first half
 */

void branch_merge_split_plugin_apply(struct abr_context *ctx, BitWindow *window)
{
    if (!window || window->length < 2) return;

    /* Branch: duplicate */
    size_t len = window->length;
    uint8_t *branch = malloc(len);
    memcpy(branch, window->bits, len);

    /* Merge: XOR halves */
    for (size_t i = 0; i < len / 2; ++i) {
        window->bits[i] ^= branch[i + len/2];
    }

    /* Split: keep first half */
    window->length = len / 2;
    /* bits already correct in first half */
}
