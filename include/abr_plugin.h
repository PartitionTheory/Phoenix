#ifndef ABR_PLUGIN_H
#define ABR_PLUGIN_H

/* Phoenix Structural Machine — Plugin Contract
 *
 * This header defines the structural interface for Phoenix plugins.
 * Plugins are extension modules that attach to the core machine via
 * a deterministic registration contract.
 */

#include "abr_core.h"
#include "abr_interface.h"

/* Plugin identifier type */
typedef phoenix_id phoenix_plugin_id;

/* Plugin lifecycle states */
typedef enum {
    PHOENIX_PLUGIN_STATE_UNINITIALIZED = 0,
    PHOENIX_PLUGIN_STATE_INITIALIZED,
    PHOENIX_PLUGIN_STATE_ACTIVE,
    PHOENIX_PLUGIN_STATE_SHUTDOWN
} phoenix_plugin_state;

/* Plugin descriptor */
typedef struct {
    phoenix_plugin_id id;
    const char *name;
    phoenix_plugin_state state;
} phoenix_plugin_descriptor;

/* Plugin entry points (structural declarations only) */
typedef phoenix_bool (*phoenix_plugin_init_fn)(phoenix_plugin_descriptor *desc);
typedef phoenix_bool (*phoenix_plugin_execute_fn)(const phoenix_request *req,
                                                  phoenix_response *res);
typedef void (*phoenix_plugin_shutdown_fn)(phoenix_plugin_descriptor *desc);

#endif /* ABR_PLUGIN_H */

