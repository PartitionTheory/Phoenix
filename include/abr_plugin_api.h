#ifndef ABR_PLUGIN_H
#define ABR_PLUGIN_H

#include "bitwindow.h"
#include "context.h"

/*
 * Phoenix plugin ABI
 *
 * All plugins operate on BitWindow and abr_context.
 */

/* Opaque plugin instance type (reserved for future use) */
typedef struct abr_plugin abr_plugin;

/* Plugin operation signature */
typedef void (*abr_plugin_apply_fn)(
        abr_context *ctx,
        BitWindow   *window
);

/* Plugin descriptor */
typedef struct abr_plugin_descriptor {
    const char           *name;
    const char           *description;
    abr_plugin_apply_fn   apply;
} abr_plugin_descriptor;

/* Registry-visible plugin entry */
typedef struct abr_plugin_entry {
    const abr_plugin_descriptor *desc;
} abr_plugin_entry;

#endif /* ABR_PLUGIN_H */
