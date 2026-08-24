#include <stdio.h>
#include "interface/core_tape.h"

/*
 * Core Test (Phoenix Edition)
 *
 * This file performs basic correctness tests on the core tape:
 *   - initialization
 *   - movement
 *   - read/write
 *   - reset
 *
 * It is intentionally minimal: deeper tests live in core/tests/.
 */

static int test_init()
{
    CoreTape t;
    if (core_tape_init(&t, 16) != 0)
        return -1;

    int v;
    if (core_tape_read(&t, &v) != 0)
        return -2;
    if (v != 0)
        return -3;

    core_tape_free(&t);
    return 0;
}

static int test_move()
{
    CoreTape t;
    core_tape_init(&t, 16);

    if (core_tape_move(&t, +1) != 0)
        return -1;
    if (core_tape_move(&t, +1) != 0)
        return -2;
    if (core_tape_move(&t, -1) != 0)
        return -3;

    core_tape_free(&t);
    return 0;
}

static int test_read_write()
{
    CoreTape t;
    core_tape_init(&t, 16);

    if (core_tape_write(&t, 42) != 0)
        return -1;

    int v;
    if (core_tape_read(&t, &v) != 0)
        return -2;
    if (v != 42)
        return -3;

    core_tape_free(&t);
    return 0;
}

static int test_reset()
{
    CoreTape t;
    core_tape_init(&t, 16);

    core_tape_write(&t, 99);
    core_tape_move(&t, +1);
    core_tape_write(&t, 77);

    core_tape_reset(&t);

    int v;
    if (core_tape_read(&t, &v) != 0)
        return -1;
    if (v != 0)
        return -2;

    core_tape_free(&t);
    return 0;
}

int main(void)
{
    printf("Running core tests...\n");

    if (test_init() != 0) {
        printf("test_init FAILED\n");
        return 1;
    }

    if (test_move() != 0) {
        printf("test_move FAILED\n");
        return 1;
    }

    if (test_read_write() != 0) {
        printf("test_read_write FAILED\n");
        return 1;
    }

    if (test_reset() != 0) {
        printf("test_reset FAILED\n");
        return 1;
    }

    printf("All core tests PASSED\n");
    return 0;
}
