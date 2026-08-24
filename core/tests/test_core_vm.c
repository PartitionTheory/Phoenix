#include <stdio.h>
#include <assert.h>
#include "../interface/core_tape.h"

/*
 * Core VM Tests (Phoenix Edition)
 *
 * These tests simulate VM-like usage patterns:
 *
 *   - instruction-style move/write/read cycles
 *   - multi-step execution
 *   - repeated instruction sequences
 *   - correctness under alternating operations
 *
 * This suite ensures the tape subsystem is ready for the VM layer.
 */

static void simulate_instruction(CoreTape *t, int opcode, int arg)
{
    /*
     * Fake instruction semantics:
     *
     *   opcode 1: write arg
     *   opcode 2: move right
     *   opcode 3: move left
     *   opcode 4: add arg to current cell
     */

    int v;

    switch (opcode) {
        case 1: /* write */
            assert(core_tape_write(t, arg) == 0);
            break;

        case 2: /* move right */
            assert(core_tape_move(t, +1) == 0);
            break;

        case 3: /* move left */
            assert(core_tape_move(t, -1) == 0);
            break;

        case 4: /* add */
            assert(core_tape_read(t, &v) == 0);
            assert(core_tape_write(t, v + arg) == 0);
            break;

        default:
            assert(0 && "invalid opcode");
    }
}

static void test_vm_sequence()
{
    CoreTape t;
    core_tape_init(&t, 16);

    /* Simulate a small VM program */
    simulate_instruction(&t, 1, 10); /* write 10 */
    simulate_instruction(&t, 2, 0);  /* move right */
    simulate_instruction(&t, 1, 20); /* write 20 */
    simulate_instruction(&t, 4, 5);  /* add 5 → 25 */

    int v;

    /* Verify */
    assert(core_tape_read(&t, &v) == 0);
    assert(v == 25);

    simulate_instruction(&t, 3, 0); /* move left */

    assert(core_tape_read(&t, &v) == 0);
    assert(v == 10);

    core_tape_free(&t);
}

static void test_vm_repeated()
{
    CoreTape t;
    core_tape_init(&t, 16);

    /* Run repeated instruction cycles */
    for (int i = 0; i < 5; i++) {
        core_tape_reset(&t);

        simulate_instruction(&t, 1, i); /* write i */
        simulate_instruction(&t, 2, 0); /* move right */
        simulate_instruction(&t, 1, i * 10); /* write 10i */

        int v;
        assert(core_tape_read(&t, &v) == 0);
        assert(v == i * 10);
    }

    core_tape_free(&t);
}

int main(void)
{
    printf("Running core VM tests...\n");

    test_vm_sequence();
    test_vm_repeated();

    printf("All core VM tests PASSED\n");
    return 0;
}
