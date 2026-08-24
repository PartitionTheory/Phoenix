#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../system_modes.h"
#include "../../vm/instruction.h"
#include "../../vm/vm_plugin.h"
#include "../../core/interface/core_tape.h"

/*
 * Phoenix System Modes + Pipeline Integration Tests
 *
 * These tests validate:
 *   - full multi-phase pipeline execution under all system modes
 *   - plugin behavior per mode
 *   - deterministic transformations
 *   - cross-cell data flow
 *   - mode-specific constraints
 */

/* -----------------------------------------------------------
 * Plugin instruction: halve cell value
 * -----------------------------------------------------------
 */
static int instr_half(CoreTape *t, int arg)
{
    (void)arg;
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v / 2);
}

static int plugin_half_init(void)
{
    return vm_register_instruction((VMOpcode)120, "half", instr_half);
}

/* -----------------------------------------------------------
 * NORMAL mode integration pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_normal()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 6) == 0);

    /* Normal mode: plugins NOT auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_NORMAL) == 0);
    assert(sys.plugins_loaded == 0);

    /*
     * Pipeline:
     *   write 10
     *   add 5        → 15
     *   move right
     *   write 8
     *   sub 3        → 5
     *   move left
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 10) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 5) == 0);
    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 3, VM_WRITE, 8) == 0);
    assert(system_load_step(&sys, 4, VM_SUB, 3) == 0);
    assert(system_load_step(&sys, 5, VM_MOVE_LEFT, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    /* tape[0] == 15 */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 15);

    /* tape[1] == 5 */
    sys.vm.tape.impl.head = 1;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 5);

    /* head returned to 0 */
    assert(sys.vm.tape.impl.head == 0);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * PLUGIN mode integration pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_plugin()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 4) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_half_init) == 0);

    /* Plugin mode: plugins auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_PLUGIN) == 0);
    assert(sys.plugins_loaded == 1);

    /*
     * Pipeline:
     *   write 20
     *   half        → 10
     *   add 4       → 14
     *   half        → 7
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 20) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)120, 0) == 0);
    assert(system_load_step(&sys, 2, VM_ADD, 4) == 0);
    assert(system_load_step(&sys, 3, (VMOpcode)120, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 7);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * SAFE mode integration pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_safe()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 3) == 0);

    /* Safe mode: plugins disabled */
    assert(system_mode_apply(&sys, SYSTEM_MODE_SAFE) == 0);
    assert(sys.plugins_loaded == 0);

    /*
     * Pipeline:
     *   write 12
     *   add 3        → 15
     *   move right
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 12) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 3) == 0);
    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 15);

    assert(sys.vm.tape.impl.head == 1);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * DIAGNOSTIC mode integration pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_diagnostic()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 4) == 0);

    /* Diagnostic mode: plugins allowed + logging */
    assert(system_mode_apply(&sys, SYSTEM_MODE_DIAGNOSTIC) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_half_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /*
     * Pipeline:
     *   write 6
     *   half        → 3
     *   add 9       → 12
     *   half        → 6
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 6) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)120, 0) == 0);
    assert(system_load_step(&sys, 2, VM_ADD, 9) == 0);
    assert(system_load_step(&sys, 3, (VMOpcode)120, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 6);

    system_free(&sys);
}

int main(void)
{
    printf("Running system modes pipeline integration tests...\n");

    test_modes_pipeline_integration_normal();
    test_modes_pipeline_integration_plugin();
    test_modes_pipeline_integration_safe();
    test_modes_pipeline_integration_diagnostic();

    printf("All system modes pipeline integration tests PASSED\n");
    return 0;
}
