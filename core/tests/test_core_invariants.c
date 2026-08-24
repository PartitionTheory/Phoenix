#include <stdio.h>
#include <assert.h>
#include "../interface/core_tape.h"

/*
 * Core Invariant Tests (Phoenix Edition)
 *
 * These tests validate fundamental invariants:
 *
 *   1. Head position invariant:
 *        0 <= head < capacity
 *
 *   2. Reset invariant:
 *        After reset, all cells == 0 and head == 0
 *
 *   3. Write-read invariant:
 *        Writing v at head then reading returns v
 *
 *   4. Movement invariant:
 *        Moving left/right updates head correctly
 *
 *   5. Stability invariant:
 *        Values at other cells remain unchanged
 *
 * These invariants must hold for all tape operations.
 */

static void invariant_head_bounds()
{
    CoreTape t;
    core_tape_init(&t, 10);

    /* Valid moves */
    assert(core_tape_move(&t, +1) == 0);
    assert(t.impl.head == 1);

    assert(core_tape_move(&t, +1) == 0);
    assert(t.impl.head == 2);

    /* Invalid moves */
    assert(core_tape_move(&t, -100) != 0);
    assert(core_tape_move(&t, +100) != 0);

    /* Head remains unchanged after invalid moves */
    assert(t.impl.head == 2);

    core_tape_free(&t);
}

static void invariant_reset()
{
    CoreTape t;
    core_tape_init(&t, 10);

    /* Write values */
    core_tape_write(&t, 5);
    core_tape_move(&t, +1);
    core_tape_write(&t, 7);

    /* Reset */
    core_tape_reset(&t);

    /* Check invariants */
    assert(t.impl.head == 0);

    for (size_t i = 0; i < t.impl.capacity; i++) {
        int v;
        t.impl.head = i;
        assert(core_tape_read(&t, &v) == 0);
        assert(v == 0);
    }

    core_tape_free(&t);
}

static void invariant_write_read()
{
    CoreTape t;
    core_tape_init(&t, 10);

    /* Write and read */
    assert(core_tape_write(&t, 42) == 0);

    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 42);

    /* Move and write again */
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_write(&t, 99) == 0);

    assert(core_tape_read(&t, &v) == 0);
    assert(v == 99);

    core_tape_free(&t);
}

static void invariant_stability()
{
    CoreTape t;
    core_tape_init(&t, 10);

    /* Write at head */
    assert(core_tape_write(&t, 11) == 0);

    /* Move and write */
    assert(core_tape_move(&t, +1) == 0);
    assert(core_tape_write(&t, 22) == 0);

    /* Move back and verify stability */
    assert(core_tape_move(&t, -1) == 0);

    int v;
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 11);

    core_tape_free(&t);
}

int main(void)
{
    printf("Running core invariant tests...\n");

    invariant_head_bounds();
    invariant_reset();
    invariant_write_read();
    invariant_stability();

    printf("All core invariant tests PASSED\n");
    return 0;
}
