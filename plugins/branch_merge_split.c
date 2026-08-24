#include "branch_merge_split.h"
#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI implementation: branch / merge / split plugin
 *
 * Currently a no-op on BitWindow->value.
 */

void branch_merge_split_plugin_apply(struct abr_context *ctx, BitWindow *window)
{
    (void)ctx;
    (void)window;
    /* TODO: implement branch/merge/split over window->value using BigInt API */
}
