#include "flip_stretch.h"
#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI implementation: flip + stretch plugin
 *
 * Currently a no-op on BitWindow->value.
 */

void flip_stretch_plugin_apply(abr_context *ctx, BitWindow *window)
{
    (void)ctx;
    (void)window;
    /* TODO: implement flip/stretch over window->value using BigInt API */
}
