#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../system_modes.h"
#include "../../vm/instruction.h"
#include "../../vm/vm_plugin.h"
#include "../../core/interface/core_tape.h"

/*
 * Phoenix Advanced Modes + Pipeline Integration Tests
 *
 * These tests validate:
 *   - complex multi-phase pipelines under all system modes
 *   - chained plugin transformations
 *   - multi-cell, multi-stage execution
 *   - deterministic behavior under stress
 */

/* -----------------------------------------------------------
 * Plugin instruction: multiply cell by arg
 * -----------------------------------------------------------
 */
static int instr_mul(CoreTape *t, int arg)
{
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v * arg);
}

static int plugin_mul_init(void)
{
    return vm_register_instruction((VMOpcode)130, "mul", instr_mul);
}

/* -----------------------------------------------------------
 * NORMAL mode advanced pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_advanced_normal()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 128, 10) == 0);

    /* Normal mode: plugins NOT auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_NORMAL) == 0);
    assert(sys.plugins_loaded == 0);

    /*
     * Pipeline:
     *   write 4
     *   add 6        → 10
     *   move right
     *   write 3
     *   add 7        → 10
     *   move right
     *   write 2
     *   add 8        → 10
     *   move left
     *   move left
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 4) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 6) == 0);

    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 3, VM_WRITE, 3) == 0);
    assert(system_load_step(&sys, 4, VM_ADD, 7) == 0);

    assert(system_load_step(&sys, 5, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 6, VM_WRITE, 2) == 0);
    assert(system_load_step(&sys, 7, VM_ADD, 8) == 0);

    assert(system_load_step(&sys, 8, VM_MOVE_LEFT, 0) == 0);
    assert(system_load_step(&sys, 9, VM_MOVE_LEFT, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    /* tape[0] == 10 */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    /* tape[1] == 10 */
    sys.vm.tape.impl.head = 1;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    /* tape[2] == 10 */
    sys.vm.tape.impl.head = 2;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    /* head returned to 0 */
    assert(sys.vm.tape.impl.head == 0);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * PLUGIN mode advanced pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_advanced_plugin()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 128, 6) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_mul_init) == 0);

    /* Plugin mode: plugins auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_PLUGIN) == 0);
    assert(sys.plugins_loaded == 1);

    /*
     * Pipeline:
     *   write 2
     *   mul 5       → 10
     *   mul 3       → 30
     *   add 7       → 37
     *   mul 2       → 74
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 2) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)130, 5) == 0);
    assert(system_load_step(&sys, 2, (VMOpcode)130, 3) == 0);
    assert(system_load_step(&sys, 3, VM_ADD, 7) == 0);
    assert(system_load_step(&sys, 4, (VMOpcode)130, 2) == 0);

    /* Step 5 unused */
    assert(system_load_step(&sys, 5, VM_NOP, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 74);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * SAFE mode advanced pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_advanced_safe()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 128, 4) == 0);

    /* Safe mode: plugins disabled */
    assert(system_mode_apply(&sys, SYSTEM_MODE_SAFE) == 0);
    assert(sys.plugins_loaded == 0);

    /*
     * Pipeline:
     *   write 9
     *   add 1        → 10
     *   move right
     *   write 5
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 9) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 1) == 0);
    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 3, VM_WRITE, 5) == 0);

    assert(system_run(&sys) == 0);

    int v;

    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    sys.vm.tape.impl.head = 1;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 5);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * DIAGNOSTIC mode advanced pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_advanced_diagnostic()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 128, 6) == 0);

    /* Diagnostic mode: plugins allowed + logging */
    assert(system_mode_apply(&sys, SYSTEM_MODE_DIAGNOSTIC) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_mul_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /*
     * Pipeline:
     *   write 3
     *   mul 4       → 12
     *   add 6       → 18
     *   mul 10      → 180
     *   sub 20      → 160
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 3) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)130, 4) == 0);
    assert(system_load_step(&sys, 2, VM_ADD, 6) == 0);
    assert(system_load_step(&sys, 3, (VMOpcode)130, 10) == 0);
    assert(system_load_step(&sys, 4, VM_SUB, 20) == 0);

    /* Step 5 unused */
    assert(system_load_step(&sys, 5, VM_NOP, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 160);

    system_free(&sys);
}

int main(void)
{
    printf("Running advanced system modes pipeline integration tests...\n");

    test_modes_pipeline_integration_advanced_normal();
    test_modes_pipeline_integration_advanced_plugin();
    test_modes_pipeline_integration_advanced_safe();
    test_modes_pipeline_integration_advanced_diagnostic();

    printf("All advanced system modes pipeline integration tests PASSED\n");
    return 0;
}
