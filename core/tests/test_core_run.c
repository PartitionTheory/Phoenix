#include <stdio.h>
#include <assert.h>
#include "../interface/core_tape.h"

/*
 * Core Run Tests (Phoenix Edition)
 *
 * These tests simulate realistic execution patterns:
 *
 *   - alternating move/write sequences
 *   - multi-step execution cycles
 *   - repeated runs
 *   - long-run stability
 *
 * This suite ensures the tape behaves correctly under
 * VM-like usage patterns.
 */

static void run_cycle(CoreTape *t, int start_value)
{
    /* Write at head */
    assert(core_tape_write(t, start_value) == 0);

    /* Move right and write */
    assert(core_tape_move(t, +1) == 0);
    assert(core_tape_write(t, start_value + 1) == 0);

    /* Move right and write */
    assert(core_tape_move(t, +1) == 0);
    assert(core_tape_write(t, start_value + 2) == 0);

    /* Move left twice and verify */
    assert(core_tape_move(t, -1) == 0);

    int v;
    assert(core_tape_read(t, &v) == 0);
    assert(v == start_value + 1);

    assert(core_tape_move(t, -1) == 0);
    assert(core_tape_read(t, &v) == 0);
    assert(v == start_value);
}

static void test_single_run()
{
    CoreTape t;
    core_tape_init(&t, 16);

    run_cycle(&t, 10);

    core_tape_free(&t);
}

static void test_multiple_runs()
{
    CoreTape t;
    core_tape_init(&t, 16);

    /* Run several cycles */
    for (int i = 0; i < 5; i++) {
        core_tape_reset(&t);
        run_cycle(&t, i * 10);
    }

    core_tape_free(&t);
}

static void test_long_run()
{
    CoreTape t;
    core_tape_init(&t, 32);

    /* Simulate long execution */
    for (int i = 0; i < 20; i++) {
        assert(core_tape_write(&t, i) == 0);

        if (i < 31)
            assert(core_tape_move(&t, +1) == 0);
    }

    /* Verify last write */
    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 19);

    core_tape_free(&t);
}

int main(void)
{
    printf("Running core run tests...\n");

    test_single_run();
    test_multiple_runs();
    test_long_run();

    printf("All core run tests PASSED\n");
    return 0;
}
