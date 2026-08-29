#ifndef PHOENIX_CLOCK_H
#define PHOENIX_CLOCK_H

/* Phoenix Structural Machine — Clock Contract
 *
 * This header defines the structural interface for the Phoenix Clock.
 * It provides a synthetic time base and ID/value mapping.
 * No logic is implemented here, only declarations.
 */

#include "abr_core.h"

/* Clock ID type */
typedef phoenix_id phoenix_clock_id;

/* Clock value type */
typedef phoenix_value phoenix_clock_value;

/* Base value for Phoenix clock */
#define PHOENIX_CLOCK_BASE 1000

/* Canonical clock IDs (Epoch-1) */
enum {
    PHOENIX_CLOCK_1 = 1,
    PHOENIX_CLOCK_2,
    PHOENIX_CLOCK_3,
    PHOENIX_CLOCK_4,
    PHOENIX_CLOCK_5,
    PHOENIX_CLOCK_6,
    PHOENIX_CLOCK_7,
    PHOENIX_CLOCK_8,
    PHOENIX_CLOCK_9,
    PHOENIX_CLOCK_10,
    PHOENIX_CLOCK_11
};

/* Structural declarations (implemented in phoenix_clock.c) */
phoenix_clock_value phoenix_clock_get(phoenix_clock_id id);
phoenix_bool        phoenix_clock_is_valid(phoenix_clock_id id);
phoenix_clock_value phoenix_clock_value_from_id(phoenix_clock_id id);

#endif /* PHOENIX_CLOCK_H */

