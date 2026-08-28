# phoenix_clock.h
#ifndef PHOENIX_CLOCK_H
#define PHOENIX_CLOCK_H

/*
 * Phoenix Synthetic Clock System (Model A — Static Deterministic Clocks)
 *
 * This header defines 11 immutable synthetic clocks.
 * Each clock is a mathematically anchored, structurally deterministic marker
 * in Phoenix synthetic time. They do not advance automatically; they are
 * fixed reference points used by greenlight, entropy audit, and epoch math.
 *
 * Core rule:
 *   For all i in {1..11}, Clock_i is a pure constant:
 *     Clock_i := f(i)
 *   where f is injective, monotonic, and independent of wall-clock time.
 *
 * These values are chosen to be:
 *   - strictly increasing
 *   - non-zero
 *   - structurally simple
 *   - easy to audit
 *
 * This file is intended for long-term historical study.
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Synthetic clock identifiers (1–11) */
typedef enum phoenix_clock_id {
    PHOENIX_CLOCK_1  = 1,
    PHOENIX_CLOCK_2  = 2,
    PHOENIX_CLOCK_3  = 3,
    PHOENIX_CLOCK_4  = 4,
    PHOENIX_CLOCK_5  = 5,
    PHOENIX_CLOCK_6  = 6,
    PHOENIX_CLOCK_7  = 7,
    PHOENIX_CLOCK_8  = 8,
    PHOENIX_CLOCK_9  = 9,
    PHOENIX_CLOCK_10 = 10,
    PHOENIX_CLOCK_11 = 11
} phoenix_clock_id;

/*
 * Synthetic clock value type.
 *
 * Rule:
 *   A clock value is a 64-bit integer used as a deterministic marker.
 *   It is not a timestamp; it is a structural constant.
 */
typedef long long phoenix_clock_value;

/*
 * Returns the deterministic synthetic value associated with a given clock id.
 *
 * Mathematical rule:
 *   value(i) = base + i
 *   where:
 *     base = 1000 (epoch-1 seed offset)
 *     i    = clock index in [1..11]
 *
 * This ensures:
 *   - strict monotonicity
 *   - easy human verification
 *   - no dependence on runtime state
 */
phoenix_clock_value phoenix_clock_get(phoenix_clock_id id);

/*
 * Returns 1 if the given clock id is valid (1–11), 0 otherwise.
 *
 * This is a structural guard, not a runtime heuristic.
 */
int phoenix_clock_is_valid(phoenix_clock_id id);

#ifdef __cplusplus
}
#endif

#endif /* PHOENIX_CLOCK_H */


# phoenix_clock.c
#include "phoenix_clock.h"

/*
 * Implementation of Phoenix Synthetic Clock System (Model A).
 *
 * All values are computed from a simple injective function:
 *
 *   value(i) = base + i
 *   base     = 1000
 *
 * This function is deliberately trivial:
 *   - It encodes that clocks are ordinal markers, not physical time.
 *   - It is easy to reason about formally.
 *   - It is easy to verify in audits and proofs.
 *
 * For historical study:
 *   The choice of base = 1000 marks "Epoch 1, Cycle 1" as the first
 *   synthetic era in Phoenix’s runtime mathematics.
 */

static const phoenix_clock_value PHOENIX_CLOCK_BASE = 1000;

/* Internal: pure function implementing value(i) = base + i. */
static phoenix_clock_value phoenix_clock_value_from_id(phoenix_clock_id id)
{
    /* Rule: only ids 1–11 are meaningful; others are structurally invalid. */
    return PHOENIX_CLOCK_BASE + (phoenix_clock_value)id;
}

phoenix_clock_value phoenix_clock_get(phoenix_clock_id id)
{
    /*
     * Deterministic rule:
     *   If id is valid, return value(id).
     *   If id is invalid, return 0 as the "null clock".
     *
     * The null clock is not part of synthetic time; it is a guard value.
     */
    if (!phoenix_clock_is_valid(id)) {
        return 0;
    }
    return phoenix_clock_value_from_id(id);
}

int phoenix_clock_is_valid(phoenix_clock_id id)
{
    /*
     * Structural validity:
     *   1 <= id <= 11
     *
     * This range is fixed by the historic Phoenix paper and must not change
     * without a new epoch-level protocol.
     */
    return (id >= PHOENIX_CLOCK_1 && id <= PHOENIX_CLOCK_11) ? 1 : 0;
}

