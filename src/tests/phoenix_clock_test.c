# phoenix_clock_test.c
#include <stdio.h>
#include <assert.h>
#include "../core/phoenix_clock.h"

/*
 * Phoenix Synthetic Clock Test Suite
 * Epoch 1 — Cycle 1 — Clock Verification
 *
 * This file validates the deterministic mathematical rules governing
 * Phoenix Synthetic Time (Clocks 1–11).
 *
 * Mathematical Rule:
 *   value(i) = base + i
 *   base     = 1000
 *
 * Therefore:
 *   Clock 1  = 1001
 *   Clock 2  = 1002
 *   ...
 *   Clock 11 = 1011
 *
 * These tests verify:
 *   - basic correctness
 *   - structural determinism
 *   - extreme boundary conditions
 *   - entropy‑free behavior
 *
 * This file is intended for long‑term historical study.
 */

/* Basic test: each clock must match deterministic formula value(i) = 1000 + i */
static void test_basic_clock_values(void)
{
    printf("[Phoenix Test] Basic Clock Values\n");

    for (int i = PHOENIX_CLOCK_1; i <= PHOENIX_CLOCK_11; i++) {
        phoenix_clock_value expected = 1000 + i;
        phoenix_clock_value actual   = phoenix_clock_get((phoenix_clock_id)i);

        assert(actual == expected);
        printf("  Clock %d OK (value = %lld)\n", i, actual);
    }
}

/* Structural test: all valid IDs must return non-zero; invalid IDs must return zero */
static void test_structural_validity(void)
{
    printf("[Phoenix Test] Structural Validity\n");

    /* Valid range */
    for (int i = PHOENIX_CLOCK_1; i <= PHOENIX_CLOCK_11; i++) {
        assert(phoenix_clock_is_valid((phoenix_clock_id)i) == 1);
        assert(phoenix_clock_get((phoenix_clock_id)i) != 0);
    }

    /* Invalid range tests */
    assert(phoenix_clock_is_valid((phoenix_clock_id)0) == 0);
    assert(phoenix_clock_get((phoenix_clock_id)0) == 0);

    assert(phoenix_clock_is_valid((phoenix_clock_id)12) == 0);
    assert(phoenix_clock_get((phoenix_clock_id)12) == 0);

    printf("  Structural validity OK\n");
}

/*
 * Extreme test: repeated queries, stress conditions, invalid spam.
 *
 * Mathematical Rule:
 *   Deterministic systems must remain stable under repeated evaluation.
 *
 * This test ensures Phoenix clocks do not drift, mutate, or degrade.
 */
static void test_extreme_conditions(void)
{
    printf("[Phoenix Test] Extreme Conditions\n");

    /* Stress test: 1 million repeated queries of Clock 1 */
    phoenix_clock_value v = 0;
    for (int i = 0; i < 1000000; i++) {
        v = phoenix_clock_get(PHOENIX_CLOCK_1);
    }
    assert(v == 1001);

    /* Stress invalid spam */
    for (int i = 0; i < 1000000; i++) {
        assert(phoenix_clock_get((phoenix_clock_id)9999) == 0);
    }

    printf("  Extreme deterministic stability OK\n");
}

/*
 * Phoenix Hello World = Clock 1
 *
 * This is the first operational output of Phoenix synthetic time.
 * It must equal 1001.
 */
static void test_hello_world(void)
{
    printf("[Phoenix Test] Hello World = Clock 1\n");

    phoenix_clock_value hw = phoenix_clock_get(PHOENIX_CLOCK_1);
    assert(hw == 1001);

    printf("  Hello World OK (value = %lld)\n", hw);
}

/*
 * Main test runner
 */
int main(void)
{
    printf("=== Phoenix Synthetic Clock Test Suite (Epoch 1) ===\n");

    test_basic_clock_values();
    test_structural_validity();
    test_extreme_conditions();
    test_hello_world();

    printf("=== All Phoenix Clock Tests Passed Deterministically ===\n");
    return 0;
}

