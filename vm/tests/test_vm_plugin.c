#include <stdio.h>
#include <assert.h>
#include "../vm_plugin.h"
#include "../instruction.h"
#include "../../core/interface/core_tape.h"
#include "../vm.h"

/*
 * Phoenix VM Plugin Tests
 *
 * These tests validate:
 *   - plugin registration
 *   - plugin initialization
 *   - plugin-provided instruction injection
 *   - VM execution using plugin instructions
 *   - failure handling
 */

/* -----------------------------------------------------------
 * A simple plugin instruction: multiply cell by arg
 * -----------------------------------------------------------
 */
static int instr_mul(CoreTape *t, int arg)
{
    int v;
    assert(core_tape_read(t, &v) == 0);
    return core_tape_write(t, v * arg);
}

/* Plugin initializer */
static int plugin_mul_init(void)
{
    return vm_register_instruction((VMOpcode)50, "mul", instr_mul);
}

/* -----------------------------------------------------------
 * A plugin that intentionally fails
 * -----------------------------------------------------------
 */
static int plugin_fail_init(void)
{
    return -1;
}

/* -----------------------------------------------------------
 * Test plugin registration and execution
 * -----------------------------------------------------------
 */
static void test_plugin_mul()
{
    /* Register plugin */
    assert(vm_plugin_register(plugin_mul_init) == 0);

    /* Run plugin initializers */
    assert(vm_plugin_run_all() == 0);

    /* Build VM */
    VM vm;
    assert(vm_init(&vm, 16, 2) == 0);

    /* Program:
     *   write 3
     *   mul 7   → 21
     */
    assert(vm_program_set(&vm.program, 0, VM_WRITE, 3) == 0);
    assert(vm_program_set(&vm.program, 1, (VMOpcode)50, 7) == 0);

    assert(vm_run(&vm) == 0);

    int v;
    vm.tape.impl.head = 0;
    assert(core_tape_read(&vm.tape, &v) == 0);
    assert(v == 21);

    vm_free(&vm);
}

/* -----------------------------------------------------------
 * Test plugin failure handling
 * -----------------------------------------------------------
 */
static void test_plugin_fail()
{
    /* Register failing plugin */
    assert(vm_plugin_register(plugin_fail_init) == 0);

    /* Running all plugins should fail */
    assert(vm_plugin_run_all() != 0);
}

int main(void)
{
    printf("Running VM plugin tests...\n");

    test_plugin_mul();
    test_plugin_fail();

    printf("All VM plugin tests PASSED\n");
    return 0;
}
