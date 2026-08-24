#include <stdio.h>
#include <assert.h>
#include "../interface/core_tape.h"

/*
 * Core Integration Tests (Phoenix Edition)
 *
 * These tests validate that the interface layer (core_tape.c)
 * and the implementation layer (tape.c) work together correctly
 * under realistic usage patterns.
 *
 * This suite simulates VM-like tape usage:
 *   - sequential writes
 *   - movement across multiple cells
 *   - read/write consistency
 *   - reset behavior
 *   - boundary correctness
 */

static void test_sequence()
{
    CoreTape t;
    assert(core_tape_init(&t, 16) == 0);

    /* Write at head */
    assert(core_tape_write(&t, 10) == 0);

    /* Move and write */
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_write(&t, 20) == 0);

    /* Move and write again */
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_write(&t, 30) == 0);

    /* Read back values */
    int v;

    assert(core_tape_move(&t, -1) == 0);
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 20);

    assert(core_tape_move(&t, -1) == 0);
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 10);

    core_tape_free(&t);
}

static void test_reset_sequence()
{
    CoreTape t;
    core_tape_init(&t, 16);

    /* Write several values */
    assert(core_tape_write(&t, 5) == 0);
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_write(&t, 6) == 0);
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_write(&t, 7) == 0);

    /* Reset */
    core_tape_reset(&t);

    /* Verify reset */
    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 0);

    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 0);

    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 0);

    core_tape_free(&t);
}

static void test_bounds_integration()
{
    CoreTape t;
    core_tape_init(&t, 4);

    /* Valid moves */
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_move(&t, +1) == 0);

    /* Invalid moves */
    assert(core_tape_move(&t, +1) != 0); /* out of bounds */
    assert(core_tape_move(&t, -3) != 0); /* out of bounds */

    core_tape_free(&t);
}

int main(void)
{
    printf("Running core integration tests...\n");

    test_sequence();
    test_reset_sequence();
    test_bounds_integration();

    printf("All core integration tests PASSED\n");
    return 0;
}
