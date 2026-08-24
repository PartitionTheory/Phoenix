#include <stdio.h>
#include <assert.h>
#include "../system.h"
#include "../system_modes.h"
#include "../../vm/instruction.h"
#include "../../vm/vm_plugin.h"
#include "../../core/interface/core_tape.h"

/*
 * Phoenix Extreme Mode + Pipeline Integration Test
 *
 * This test pushes the system through:
 *   - DIAGNOSTIC mode
 *   - plugin loading
 *   - multi‑cell pipeline execution
 *   - extreme value transformations
 *   - cross‑cell consistency checks
 */

/* -----------------------------------------------------------
 * Extreme plugin: multiply cell by -7
 * -----------------------------------------------------------
 */
static int instr_mulneg7(CoreTape *t, int arg)
{
    (void)arg;
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v * -7);
}

static int plugin_mulneg7_init(void)
{
    return vm_register_instruction((VMOpcode)111, "mulneg7", instr_mulneg7);
}

/* -----------------------------------------------------------
 * Extreme integration test
 * -----------------------------------------------------------
 */
static void test_modes_pipeline_integration_extreme()
{
    PhoenixSystem sys;
    assert(system_init(&sys, 128, 6) == 0);

    /* Diagnostic mode: plugins allowed */
    assert(system_mode_apply(&sys, SYSTEM_MODE_DIAGNOSTIC) == 0);

    /* Register extreme plugin */
    assert(vm_plugin_register(plugin_mulneg7_init) == 0);
    assert(system_load_plugins(&sys) == 0);

    /*
     * Extreme pipeline:
     *   write 3
     *   mulneg7        → -21
     *   add 1000       → 979
     *   move right
     *   write -2
     *   mulneg7        → 14
     */

    assert(system_load_step(&sys, 0, VM_WRITE, 3) == 0);
    assert(system_load_step(&sys, 1, (VMOpcode)111, 0) == 0);
    assert(system_load_step(&sys, 2, VM_ADD, 1000) == 0);
    assert(system_load_step(&sys, 3, VM_MOVE_RIGHT, 0) == 0);
    assert(system_load_step(&sys, 4, VM_WRITE, -2) == 0);
    assert(system_load_step(&sys, 5, (VMOpcode)111, 0) == 0);

    assert(system_run(&sys) == 0);

    int v;

    /* Cell 0: 3 → -21 → 979 */
    sys.vm.tape.impl.head = 0;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 979);

    /* Cell 1: -2 → 14 */
    sys.vm.tape.impl.head = 1;
    assert(core_tape_read(&sys.vm.tape, &v) == 0);
    assert(v == 14);

    system_free(&sys);
}

/* -----------------------------------------------------------
 * Main entry point
 * -----------------------------------------------------------
 */
int main(void)
{
    printf("Running EXTREME system modes + pipeline integration test...\n");
    test_modes_pipeline_integration_extreme();
    printf("EXTREME test PASSED\n");
    return 0;
}
