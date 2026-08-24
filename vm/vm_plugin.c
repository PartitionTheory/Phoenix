#include <stdlib.h>
#include "vm_plugin.h"

/*
 * Phoenix VM Plugin System
 *
 * Responsibilities:
 *   - store plugin initialization functions
 *   - run all plugin initializers
 *   - enable dynamic instruction registration
 *   - allow domain-specific VM extensions
 */

#define MAX_PLUGINS 64

static VMPluginInitFunc plugin_inits[MAX_PLUGINS];
static size_t plugin_count = 0;

/* -----------------------------------------------------------
 * Register plugin initializer
 * -----------------------------------------------------------
 */
int vm_plugin_register(VMPluginInitFunc init_func)
{
    if (plugin_count >= MAX_PLUGINS)
        return -1;

    plugin_inits[plugin_count++] = init_func;
    return 0;
}

/* -----------------------------------------------------------
 * Run all plugin initializers
 * -----------------------------------------------------------
 */
int vm_plugin_run_all(void)
{
    for (size_t i = 0; i < plugin_count; i++) {
        int status = plugin_inits[i]();
        if (status != 0)
            return status;
    }
    return 0;
}
