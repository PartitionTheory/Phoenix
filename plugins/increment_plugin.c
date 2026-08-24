#include "increment_plugin.h"
#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI implementation: increment bits in window
 */

void increment_plugin_apply(struct abr_context *ctx, BitWindow *window)
{
    if (!window) return;

    for (size_t i = 0; i < window->length; ++i) {
        window->bits[i] ^= 1; /* simple increment: flip bit */
    }
}
