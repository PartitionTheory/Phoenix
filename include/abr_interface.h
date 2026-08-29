#ifndef ABR_INTERFACE_H
#define ABR_INTERFACE_H

/* Phoenix Structural Machine — External Interface Contract
 *
 * This header defines the structural interface used by external systems
 * to communicate with Phoenix subsystems. It contains no logic.
 * It is the outward-facing contract of the structural machine.
 */

#include "abr_core.h"

/* Phoenix subsystem identifiers */
typedef enum {
    PHOENIX_SUBSYSTEM_CORE = 1,
    PHOENIX_SUBSYSTEM_VM,
    PHOENIX_SUBSYSTEM_INTERFACE,
    PHOENIX_SUBSYSTEM_PLUGINS,
    PHOENIX_SUBSYSTEM_TESTS
} phoenix_subsystem_id;

/* Phoenix interface request structure */
typedef struct {
    phoenix_subsystem_id target;
    phoenix_id command;
    phoenix_value payload;
} phoenix_request;

/* Phoenix interface response structure */
typedef struct {
    phoenix_bool success;
    phoenix_value result;
} phoenix_response;

#endif /* ABR_INTERFACE_H */
 
