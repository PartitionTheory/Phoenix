#include "../../include/abr_plugin.h"

/* Phoenix Structural Machine — Plugin Engine (Epoch‑1)
 *
 * Minimal deterministic lifecycle for Phoenix plugins.
 * No dynamic loading, no registry, no advanced routing.
 */

phoenix_bool phoenix_plugin_init(phoenix_plugin_descriptor *desc)
{
    if (desc == NULL)
        return PHOENIX_FALSE;

    desc->state = PHOENIX_PLUGIN_STATE_INITIALIZED;
    return PHOENIX_TRUE;
}

phoenix_bool phoenix_plugin_execute(const phoenix_request *req,
                                    phoenix_response *res)
{
    if (req == NULL || res == NULL)
        return PHOENIX_FALSE;

    /* Epoch‑1 rule:
     * result = payload
     * success = TRUE
     */
    res->success = PHOENIX_TRUE;
    res->result  = req->payload;

    return PHOENIX_TRUE;
}

void phoenix_plugin_shutdown(phoenix_plugin_descriptor *desc)
{
    if (desc == NULL)
        return;

    desc->state = PHOENIX_PLUGIN_STATE_SHUTDOWN;
}

