#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../../vm/instruction.h"
#include "../../core/interface/core_tape.h"
#include "../../vm/vm_plugin.h"

/*
 * Phoenix System Integration Tests
 *
 * These tests validate:
 *   - full system orchestration
 *   - VM + program + tape + plugins working together
 *   - multi-step execution correctness
 *   - plugin instruction integration
 *   - reset semantics
 */

/* -----------------------------------------------------------
 * Plugin instruction: triple cell value
 * -----------------------------------------------------------
 */
static int instr_triple(CoreTape *t, int arg)
{
    (void)arg;
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v * 3);
}

static int plugin_triple_init(void)
{
    return vm_register_instruction((VMOpcode)80, "triple", instr_triple);
}

/* -----------------------------------------------------------
 * Full integration test WITHOUT plugins
 * -----------------------------------------------------------
 */
static void test_system_integration_basic()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 32, 6) == 0);

    /*
     * Program:
     *   write 1
     *   add 4        → 5
     *   move right
     *   write 10
     *   sub 3        → 7
     *   move left
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 1) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 4) == 0);
    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 3, VM_WRITE, 10) == 0);
    assert(system_load_step(&sys, 4, VM_SUB, 3) == 0);
    assert(system_load_step(&sys, 5, VM_MOVE_LEFT, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    /* tape[0] should be 5 */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 5);

    /* tape[1] should be 7 */
    sys.vm.tape.impl.head = 1;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 7);

    /* head should be back at 0 */
    assert(sys.vm.tape.impl.head == 0);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Full integration test WITH plugin instruction
 * -----------------------------------------------------------
 */
static void test_system_integration_plugin()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 32, 3) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_triple_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /*
     * Program:
     *   write 2
     *   triple      → 6
     *   triple      → 18
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 2) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)80, 0) == 0);
    assert(system_load_step(&sys, 2, (VMOpcode)80, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 18);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * System reset integration test
 * -----------------------------------------------------------
 */
static void test_system_integration_reset()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 32, 2) == 0);

    assert(system_load_step(&sys, 0, VM_WRITE, 42) == 0);
    assert(system_load_step(&sys, 1, VM_MOVE_RIGHT, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    /* tape[0] should be 42 */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 42);

    /* head should be 1 */
    assert(sys.vm.tape.impl.head == 1);

    /* Reset system */
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
    printf("Running system integration tests...\n");

    test_system_integration_basic();
    test_system_integration_plugin();
    test_system_integration_reset();

    printf("All system integration tests PASSED\n");
    return 0;
}
