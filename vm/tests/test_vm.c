#include <stdio.h>
#include <assert.h>
#include "../../core/interface/core_tape.h"
#include "../vm.h"
#include "../instruction.h"

/*
 * Phoenix VM Tests
 *
 * These tests validate:
 *   - VM initialization
 *   - program construction
 *   - instruction execution
 *   - tape correctness after VM run
 *   - error handling
 */

static void test_vm_basic()
{
    VM vm;
    assert(vm_init(&vm, 16, 3) == 0);

    /* Program:
     *   write 10
     *   move right
     *   write 20
     */
    assert(vm_program_set(&vm.program, 0, VM_WRITE, 10) == 0);
    assert(vm_program_set(&vm.program, 1, VM_MOVE_RIGHT, 0) == 0);
    assert(vm_program_set(&vm.program, 2, VM_WRITE, 20) == 0);

    assert(vm_run(&vm) == 0);

    int v;

    /* Verify tape[0] == 10 */
    vm.tape.impl.head = 0;
    assert(core_tape_read(&vm.tape, &v) == 0);
    assert(v == 10);

    /* Verify tape[1] == 20 */
    vm.tape.impl.head = 1;
    assert(core_tape_read(&vm.tape, &v) == 0);
    assert(v == 20);

    vm_free(&vm);
}

static void test_vm_add_sub()
{
    VM vm;
    assert(vm_init(&vm, 16, 4) == 0);

    /* Program:
     *   write 5
     *   add 7      → 12
     *   sub 2      → 10
     *   move right
     */
    assert(vm_program_set(&vm.program, 0, VM_WRITE, 5) == 0);
    assert(vm_program_set(&vm.program, 1, VM_ADD, 7) == 0);
    assert(vm_program_set(&vm.program, 2, VM_SUB, 2) == 0);
    assert(vm_program_set(&vm.program, 3, VM_MOVE_RIGHT, 0) == 0);

    assert(vm_run(&vm) == 0);

    int v;

    /* Verify tape[0] == 10 */
    vm.tape.impl.head = 0;
    assert(core_tape_read(&vm.tape, &v) == 0);
    assert(v == 10);

    /* Verify head moved right */
    assert(vm.tape.impl.head == 1);

    vm_free(&vm);
}

static void test_vm_unknown_opcode()
{
    VM vm;
    assert(vm_init(&vm, 16, 1) == 0);

    /* Invalid opcode */
    assert(vm_program_set(&vm.program, 0, (VMOpcode)999, 0) == 0);

    /* VM should fail */
    assert(vm_run(&vm) != 0);

    vm_free(&vm);
}

int main(void)
{
    printf("Running VM tests...\n");

    test_vm_basic();
    test_vm_add_sub();
    test_vm_unknown_opcode();

    printf("All VM tests PASSED\n");
    return 0;
}
