#include <stdio.h>
#include <assert.h>
#include "../interface/core_tape.h"

/*
 * Deep Core Tests (Phoenix Edition)
 *
 * These tests validate:
 *   - tape initialization
 *   - tape movement invariants
 *   - tape read/write invariants
 *   - tape reset invariants
 *   - boundary conditions
 *
 * This suite is more thorough than core/test_core.c.
 */

static void test_init()
{
    CoreTape t;
    assert(core_tape_init(&t, 32) == 0);

    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 0);

    core_tape_free(&t);
}

static void test_move_bounds()
{
    CoreTape t;
    core_tape_init(&t, 8);

    /* Move within bounds */
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_move(&t, -1) == 0);

    /* Move out of bounds */
    assert(core_tape_move(&t, -100) != 0);
    assert(core_tape_move(&t, +100) != 0);

    core_tape_free(&t);
}

static void test_read_write()
{
    CoreTape t;
    core_tape_init(&t, 8);

    /* Write at head */
    assert(core_tape_write(&t, 123) == 0);

    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 123);

    /* Move and write */
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_write(&t, 77) == 0);

    assert(core_tape_read(&t, &v) == 0);
    assert(v == 77);

    core_tape_free(&t);
}

static void test_reset()
{
    CoreTape t;
    core_tape_init(&t, 8);

    core_tape_write(&t, 99);
    core_tape_move(&t, +1);
    core_tape_write(&t, 55);

    core_tape_reset(&t);

    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 0);

    core_tape_free(&t);
}

static void test_stress()
{
    CoreTape t;
    core_tape_init(&t, 64);

    /* Write sequential values */
    for (int i = 0; i < 64; i++) {
        assert(core_tape_write(&t, i) == 0);
        assert(core_tape_move(&t, +1) == 0 || i == 63);
    }

    /* Reset and verify */
    core_tape_reset(&t);

    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 0);

    core_tape_free(&t);
}

int main(void)
{
    printf("Running deep core tests...\n");

    test_init();
    test_move_bounds();
    test_read_write();
    test_reset();
    test_stress();

    printf("All deep core tests PASSED\n");
    return 0;
}
