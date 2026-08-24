#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../system_modes.h"
#include "../../vm/instruction.h"
#include "../../vm/vm_plugin.h"
#include "../../core/interface/core_tape.h"

/*
 * Phoenix System Modes Tests
 *
 * These tests validate:
 *   - mode-based system configuration
 *   - plugin loading rules per mode
 *   - deterministic execution behavior
 *   - safe-mode restrictions
 *   - diagnostic-mode behavior
 */

/* -----------------------------------------------------------
 * Plugin instruction: increment cell
 * -----------------------------------------------------------
 */
static int instr_inc(CoreTape *t, int arg)
{
    (void)arg;
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v + 1);
}

static int plugin_inc_init(void)
{
    return vm_register_instruction((VMOpcode)100, "inc", instr_inc);
}

/* -----------------------------------------------------------
 * Test NORMAL mode
 * -----------------------------------------------------------
 */
static void test_mode_normal()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 32, 3) == 0);

    /* Normal mode: plugins NOT auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_NORMAL) == 0);
    assert(sys.plugins_loaded == 0);

    /* Program:
     *   write 7
     *   add 3      → 10
     *   move right
     */
    assert(system_load_step(&sys, 0, VM_WRITE, 7) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 3) == 0);
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
 * Test PLUGIN mode
 * -----------------------------------------------------------
 */
static void test_mode_plugin()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 32, 2) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_inc_init) == 0);

    /* Plugin mode: plugins auto-loaded */
    assert(system_mode_apply(&sys, SYSTEM_MODE_PLUGIN) == 0);
    assert(sys.plugins_loaded == 1);

    /*
     * Program:
     *   write 5
     *   inc        → 6
     */
    assert(system_load_step(&sys, 0, VM_WRITE, 5) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)100, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 6);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Test SAFE mode
 * -----------------------------------------------------------
 */
static void test_mode_safe()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 32, 2) == 0);

    /* Safe mode: plugins disabled */
    assert(system_mode_apply(&sys, SYSTEM_MODE_SAFE) == 0);
    assert(sys.plugins_loaded == 0);

    /* Program:
     *   write 9
     *   add 1      → 10
     */
    assert(system_load_step(&sys, 0, VM_WRITE, 9) == 0);
    assert(system_load_step(&sys, 1, VM_ADD, 1) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 10);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Test DIAGNOSTIC mode
 * -----------------------------------------------------------
 */
static void test_mode_diagnostic()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 32, 3) == 0);

    /* Diagnostic mode: plugins allowed, but execution logged */
    assert(system_mode_apply(&sys, SYSTEM_MODE_DIAGNOSTIC) == 0);

    /* Register plugin */
    assert(vm_plugin_register(plugin_inc_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /*
     * Program:
     *   write 1
     *   inc        → 2
     *   inc        → 3
     */
    assert(system_load_step(&sys, 0, VM_WRITE, 1) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)100, 0) == 0);
    assert(system_load_step(&sys, 2, (VMOpcode)100, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 3);

    system_free(&sys);
}

int main(void)
{
    printf("Running system mode tests...\n");

    test_mode_normal();
    test_mode_plugin();
    test_mode_safe();
    test_mode_diagnostic();

    printf("All system mode tests PASSED\n");
    return 0;
}
