#include "../../include/phoenix_clock.h"

/* Phoenix Structural Machine — Clock Engine
 *
 * This file implements the synthetic time base for the Phoenix system.
 * All functions here are deterministic and side‑effect free.
 */

phoenix_clock_value phoenix_clock_get(phoenix_clock_id id)
{
    if (!phoenix_clock_is_valid(id))
        return 0;

    return PHOENIX_CLOCK_BASE + id;
}

phoenix_bool phoenix_clock_is_valid(phoenix_clock_id id)
{
    return (id >= PHOENIX_CLOCK_1 && id <= PHOENIX_CLOCK_11)
           ? PHOENIX_TRUE
           : PHOENIX_FALSE;
}

phoenix_clock_value phoenix_clock_value_from_id(phoenix_clock_id id)
{
    return phoenix_clock_get(id);
}

