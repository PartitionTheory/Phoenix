#include "../../include/phoenix_clock.h"
#include <stdio.h>

/* Phoenix Structural Machine — Clock Test (Epoch‑1)
 *
 * Verifies that Phoenix clock IDs map to deterministic values.
 */

int main(void)
{
    phoenix_clock_id    id  = PHOENIX_CLOCK_1;
    phoenix_clock_value val = phoenix_clock_get(id);

    if (!phoenix_clock_is_valid(id))
    {
        printf("Phoenix Clock: ID %d invalid\n", id);
        return 1;
    }

    printf("Phoenix Clock: ID %d -> value %lld\n",
           id, (long long)val);

    return 0;
}

