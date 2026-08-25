#ifndef FLIP_STRETCH_H
#define FLIP_STRETCH_H

#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI: flip + stretch plugin
 */

void flip_stretch_plugin_apply(abr_context *ctx, BitWindow *window);

#endif /* FLIP_STRETCH_H */
