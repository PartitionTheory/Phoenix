#ifndef ABR_PLUGIN_REGISTRY_H
#define ABR_PLUGIN_REGISTRY_H

#include "abr_plugin.h"

/*
 * Phoenix plugin registry
 *
 * Simple static registry for built-in plugins.
 */

typedef struct abr_plugin_registry {
    const abr_plugin_entry *entries;
    int                     count;
} abr_plugin_registry;

/* Get global registry */
const abr_plugin_registry *abr_get_plugin_registry(void);

/* Lookup by name (returns NULL if not found) */
const abr_plugin_entry *abr_find_plugin(const char *name);

#endif /* ABR_PLUGIN_REGISTRY_H */
