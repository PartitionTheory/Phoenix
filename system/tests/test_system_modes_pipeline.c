#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../system_modes.h"
#include "../../vm/instruction.h"
#include "../../vm/vm_plugin.h"
#include "../../core/interface/core_tape.h"

/*
 * Phoenix System Modes + Pipeline Tests
 *
 * These tests validate:
 *   - multi-phase pipeline execution under different modes
 *   - plugin loading rules per mode
 *   - deterministic transformations
 *   - cross-cell data flow
 */

/* -----------------------------------------------------------
 * Plugin instruction: negate cell value
 * -----------------------------------------------------------
 */
static int instr_neg(CoreTape *t, int arg)
{
    (void)arg;
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, -v);
}

static int plugin_neg_init(void)
{
    return vm_register_instruction((VMOpcode)110, "neg", instr_neg);
}

/* -----------------------------------------------------------
 * NORMAL mode pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_normal()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 5) == 0);

    /* Normal mode: plugins NOT auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_NORMAL) == 0);
    assert(sys.plugins_loaded == 0);

    /*
     * Pipeline:
     *   write 8
     *   add 2        → 10
     *   move right
     *   write 3
     *   add 4        → 7
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 8) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 2) == 0);
    assert(system_load_step(&sys, 2, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 3, VM_WRITE, 3) == 0);
    assert(system_load_step(&sys, 4, VM_ADD, 4) == 0);

    assert(system_run(&sys) == 0);

    int v;

    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    sys.vm.tape.impl.head = 1;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 7);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * PLUGIN mode pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_plugin()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 4) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_neg_init) == 0);

    /* Plugin mode: plugins auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_PLUGIN) == 0);
    assert(sys.plugins_loaded == 1);

    /*
     * Pipeline:
     *   write 5
     *   neg         → -5
     *   add 10      → 5
     *   neg         → -5
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 5) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)110, 0) == 0);
    assert(system_load_step(&sys, 2, VM_ADD, 10) == 0);
    assert(system_load_step(&sys, 3, (VMOpcode)110, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == -5);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * SAFE mode pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_safe()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 3) == 0);

    /* Safe mode: plugins disabled */
    assert(system_mode_apply(&sys, SYSTEM_MODE_SAFE) == 0);
    assert(sys.plugins_loaded == 0);

    /*
     * Pipeline:
     *   write 9
     *   add 1        → 10
     *   move right
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 9) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 1) == 0);
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
 * DIAGNOSTIC mode pipeline
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_diagnostic()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 64, 4) == 0);

    /* Diagnostic mode: plugins allowed + logging */
    assert(system_mode_apply(&sys, SYSTEM_MODE_DIAGNOSTIC) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_neg_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /*
     * Pipeline:
     *   write 2
     *   neg         → -2
     *   add 5       → 3
     *   neg         → -3
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 2) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)110, 0) == 0);
    assert(system_load_step(&sys, 2, VM_ADD, 5) == 0);
    assert(system_load_step(&sys, 3, (VMOpcode)110, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == -3);

    system_free(&sys);
}

int main(void)
{
    printf("Running system modes pipeline tests...\n");

    test_modes_pipeline_normal();
    test_modes_pipeline_plugin();
    test_modes_pipeline_safe();
    test_modes_pipeline_diagnostic();

    printf("All system modes pipeline tests PASSED\n");
    return 0;
}
