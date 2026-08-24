#include <stdio.h>
#include <assert.h>
#include "../program.h"
#include "../instruction.h"

/*
 * Phoenix VM Program Tests
 *
 * These tests validate:
 *   - program initialization
 *   - step assignment
 *   - program counter correctness
 *   - current-step retrieval
 *   - reset semantics
 *   - boundary behavior
 */

static void test_program_init()
{
    VMProgram p;
    assert(vm_program_init(&p, 5) == 0);

    assert(p.length == 5);
    assert(p.pc == 0);

    vm_program_free(&p);
}

static void test_program_set_and_get()
{
    VMProgram p;
    vm_program_init(&p, 3);

    assert(vm_program_set(&p, 0, VM_WRITE, 10) == 0);
    assert(vm_program_set(&p, 1, VM_MOVE_RIGHT, 0) == 0);
    assert(vm_program_set(&p, 2, VM_ADD, 5) == 0);

    const VMProgramStep *s;

    s = vm_program_current(&p);
    assert(s->opcode == VM_WRITE);
    assert(s->arg == 10);

    vm_program_advance(&p);
    s = vm_program_current(&p);
    assert(s->opcode == VM_MOVE_RIGHT);

    vm_program_advance(&p);
    s = vm_program_current(&p);
    assert(s->opcode == VM_ADD);
    assert(s->arg == 5);

    vm_program_free(&p);
}

static void test_program_reset()
{
    VMProgram p;
    vm_program_init(&p, 2);

    vm_program_set(&p, 0, VM_WRITE, 1);
    vm_program_set(&p, 1, VM_WRITE, 2);

    vm_program_advance(&p);
    assert(p.pc == 1);

    vm_program_reset(&p);
    assert(p.pc == 0);

    const VMProgramStep *s = vm_program_current(&p);
    assert(s->opcode == VM_WRITE);
    assert(s->arg == 1);

    vm_program_free(&p);
}

static void test_program_bounds()
{
    VMProgram p;
    vm_program_init(&p, 2);

    /* Valid */
    assert(vm_program_set(&p, 0, VM_WRITE, 99) == 0);

    /* Invalid index */
    assert(vm_program_set(&p, 5, VM_WRITE, 99) != 0);

    /* Current step becomes NULL when pc >= length */
    vm_program_advance(&p);
    vm_program_advance(&p);

    assert(vm_program_current(&p) == NULL);

    vm_program_free(&p);
}

int main(void)
{
    printf("Running VM program tests...\n");

    test_program_init();
    test_program_set_and_get();
    test_program_reset();
    test_program_bounds();

    printf("All VM program tests PASSED\n");
    return 0;
}
