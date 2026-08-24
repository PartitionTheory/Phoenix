#include <stdio.h>
#include <assert.h>
#include "../../core/interface/core_tape.h"
#include "../vm.h"
#include "../instruction.h"
#include "../vm_plugin.h"

/*
 * Phoenix VM Integration Tests
 *
 * These tests validate:
 *   - full VM stack integration
 *   - tape + program + instruction registry working together
 *   - multi-step execution correctness
 *   - plugin instruction integration
 */

/* -----------------------------------------------------------
 * Optional plugin instruction: increment cell
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
    return vm_register_instruction((VMOpcode)60, "inc", instr_inc);
}

/* -----------------------------------------------------------
 * Test full VM integration without plugins
 * -----------------------------------------------------------
 */
static void test_vm_integration_basic()
{
    VM vm;
    assert(vm_init(&vm, 16, 5) == 0);

    /*
     * Program:
     *   write 2
     *   add 3       → 5
     *   move right
     *   write 7
     *   sub 2       → 5
     */

    assert(vm_program_set(&vm.program, 0, VM_WRITE, 2) == 0);
    assert(vm_program_set(&vm.program, 1, VM_ADD, 3) == 0);
    assert(vm_program_set(&vm.program, 2, VM_MOVE_RIGHT, 0) == 0);
    assert(vm_program_set(&vm.program, 3, VM_WRITE, 7) == 0);
    assert(vm_program_set(&vm.program, 4, VM_SUB, 2) == 0);

    assert(vm_run(&vm) == 0);

    int v;

    /* Verify tape[0] == 5 */
    vm.tape.impl.head = 0;
    assert(core_tape_read(&vm.tape, &v) == 0);
    assert(v == 5);

    /* Verify tape[1] == 5 */
    vm.tape.impl.head = 1;
    assert(core_tape_read(&vm.tape, &v) == 0);
    assert(v == 5);

    vm_free(&vm);
}

/* -----------------------------------------------------------
 * Test full VM integration WITH plugin instruction
 * -----------------------------------------------------------
 */
static void test_vm_integration_plugin()
{
    /* Register plugin */
    assert(vm_plugin_register(plugin_inc_init) == 0);
    assert(vm_plugin_run_all() == 0);

    VM vm;
    assert(vm_init(&vm, 16, 4) == 0);

    /*
     * Program:
     *   write 10
     *   inc        → 11
     *   inc        → 12
     *   inc        → 13
     */

    assert(vm_program_set(&vm.program, 0, VM_WRITE, 10) == 0);
    assert(vm_program_set(&vm.program, 1, (VMOpcode)60, 0) == 0);
    assert(vm_program_set(&vm.program, 2, (VMOpcode)60, 0) == 0);
    assert(vm_program_set(&vm.program, 3, (VMOpcode)60, 0) == 0);

    assert(vm_run(&vm) == 0);

    int v;
    vm.tape.impl.head = 0;
    assert(core_tape_read(&vm.tape, &v) == 0);
    assert(v == 13);

    vm_free(&vm);
}

int main(void)
{
    printf("Running VM integration tests...\n");

    test_vm_integration_basic();
    test_vm_integration_plugin();

    printf("All VM integration tests PASSED\n");
    return 0;
}
