#include "../../include/abr_interface.h"

/* Phoenix Structural Machine — Interface Engine
 *
 * This file implements the Epoch‑1 external request/response chamber.
 * It provides deterministic handling of Phoenix requests.
 * No routing, no plugin dispatch, no subsystem switching.
 */

phoenix_response phoenix_interface_handle(const phoenix_request *req)
{
    phoenix_response res;

    if (req == NULL)
    {
        res.success = PHOENIX_FALSE;
        res.result  = 0;
        return res;
    }

    /* Epoch‑1 rule:
     * If command == 0 → failure
     * Otherwise → result = payload + command
     */
    if (req->command == 0)
    {
        res.success = PHOENIX_FALSE;
        res.result  = 0;
        return res;
    }

    res.success = PHOENIX_TRUE;
    res.result  = req->payload + req->command;

    return res;
}

