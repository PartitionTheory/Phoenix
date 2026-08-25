#ifndef INCREMENT_PLUGIN_H
#define INCREMENT_PLUGIN_H

#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI: increment plugin
 *
 * Currently a no-op used to validate ABI correctness.
 */

void increment_plugin_apply(abr_context *ctx, BitWindow *window);

#endif /* INCREMENT_PLUGIN_H */
