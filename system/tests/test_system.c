#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../../vm/instruction.h"
#include "../../core/interface/core_tape.h"

/*
 * Phoenix System Tests
 *
 * These tests validate:
 *   - system initialization
 *   - plugin loading through system layer
 *   - program loading through system layer
 *   - system-level execution
 *   - system reset semantics
 */

/* -----------------------------------------------------------
 * Plugin instruction: double cell value
 * -----------------------------------------------------------
 */
static int instr_double(CoreTape *t, int arg)
{
    (void)arg;
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v * 2);
}

static int plugin_double_init(void)
{
    return vm_register_instruction((VMOpcode)70, "double", instr_double);
}

/* -----------------------------------------------------------
 * Test system initialization + basic execution
 * -----------------------------------------------------------
 */
static void test_system_basic()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 16, 3) == 0);

    /* Program:
     *   write 4
     *   add 6      → 10
     *   move right
     */
    assert(system_load_step(&sys, 0, VM_WRITE, 4) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 6) == 0);
    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    assert(sys.vm.tape.impl.head == 1);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Test system plugin loading + execution
 * -----------------------------------------------------------
 */
static void test_system_plugin()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 16, 2) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_double_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /* Program:
     *   write 5
     *   double     → 10
     */
    assert(system_load_step(&sys, 0, VM_WRITE, 5) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)70, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Test system reset
 * -----------------------------------------------------------
 */
static void test_system_reset()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 16, 1) == 0);

    assert(system_load_step(&sys, 0, VM_WRITE, 99) == 0);
    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 99);

    system_reset(&sys);

    /* After reset, tape should be zeroed */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 0);

    /* Plugins flag cleared */
    assert(sys.plugins_loaded == 0);

    system_free(&sys);
}

int main(void)
{
    printf("Running system tests...\n");

    test_system_basic();
    test_system_plugin();
    test_system_reset();

    printf("All system tests PASSED\n");
    return 0;
}
