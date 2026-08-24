#include "increment_plugin.h"
#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI implementation: increment plugin
 *
 * For now, this is a no-op on BitWindow->value.
 * Behavior can be refined once BigInt operations are mapped.
 */

void increment_plugin_apply(struct abr_context *ctx, BitWindow *window)
{
    (void)ctx;
    (void)window;
    /* TODO: implement increment over window->value using BigInt API */
}
