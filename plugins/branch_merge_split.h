#ifndef BRANCH_MERGE_SPLIT_H
#define BRANCH_MERGE_SPLIT_H

#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix ABI: branch / merge / split plugin
 */

void branch_merge_split_plugin_apply(abr_context *ctx, BitWindow *window);

#endif /* BRANCH_MERGE_SPLIT_H */
