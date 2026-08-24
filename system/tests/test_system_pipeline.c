#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../../vm/instruction.h"
#include "../../vm/vm_plugin.h"
#include "../../core/interface/core_tape.h"

/*
 * Phoenix System Pipeline Tests
 *
 * These tests validate:
 *   - multi-phase pipeline execution
 *   - system + VM + tape + program + plugins working together
 *   - deterministic multi-step transformations
 *   - cross-cell data flow
 */

/* -----------------------------------------------------------
 * Plugin instruction: square cell value
 * -----------------------------------------------------------
 */
static int instr_square(CoreTape *t, int arg)
{
    (void)arg;
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v * v);
}

static int plugin_square_init(void)
{
    return vm_register_instruction((VMOpcode)90, "square", instr_square);
}

/* -----------------------------------------------------------
 * Pipeline test WITHOUT plugins
 * -----------------------------------------------------------
 */
static void test_pipeline_basic()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 8) == 0);

    /*
     * Pipeline:
     *   Phase 1:
     *     write 3
     *     add 2        → 5
     *
     *   Phase 2:
     *     move right
     *     write 4
     *     add 6        → 10
     *
     *   Phase 3:
     *     move left
     *     sub 1        → 4
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 3) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 2) == 0);

    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 3, VM_WRITE, 4) == 0);
    assert(system_load_step(&sys, 4, VM_ADD, 6) == 0);

    assert(system_load_step(&sys, 5, VM_MOVE_LEFT, 0) == 0);
    assert(system_load_step(&sys, 6, VM_SUB, 1) == 0);

    /* Step 7 unused but valid */
    assert(system_load_step(&sys, 7, VM_NOP, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    /* tape[0] should be 4 */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 4);

    /* tape[1] should be 10 */
    sys.vm.tape.impl.head = 1;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    /* head should be back at 0 */
    assert(sys.vm.tape.impl.head == 0);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Pipeline test WITH plugin instruction
 * -----------------------------------------------------------
 */
static void test_pipeline_plugin()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 5) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_square_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /*
     * Pipeline:
     *   write 5
     *   square      → 25
     *   add 3       → 28
     *   square      → 784
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 5) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)90, 0) == 0);
    assert(system_load_step(&sys, 2, VM_ADD, 3) == 0);
    assert(system_load_step(&sys, 3, (VMOpcode)90, 0) == 0);

    /* Step 4 unused */
    assert(system_load_step(&sys, 4, VM_NOP, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 784);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Pipeline reset test
 * -----------------------------------------------------------
 */
static void test_pipeline_reset()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 3) == 0);

    assert(system_load_step(&sys, 0, VM_WRITE, 9) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 1) == 0);
    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    /* tape[0] should be 10 */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    /* head should be 1 */
    assert(sys.vm.tape.impl.head == 1);

    system_reset(&sys);

    /* After reset, tape should be zeroed */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 0);

    /* head should be reset */
    assert(sys.vm.tape.impl.head == 0);

    /* plugins flag cleared */
    assert(sys.plugins_loaded == 0);

    system_free(&sys);
}

int main(void)
{
    printf("Running system pipeline tests...\n");

    test_pipeline_basic();
    test_pipeline_plugin();
    test_pipeline_reset();

    printf("All system pipeline tests PASSED\n");
    return 0;
}
