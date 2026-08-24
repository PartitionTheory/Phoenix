#include <stdlib.h>
#include "vm.h"

/*
 * Phoenix VM Execution Engine
 *
 * Responsibilities:
 *   - initialize tape + program
 *   - execute program step-by-step
 *   - fetch instruction from registry
 *   - apply instruction to tape
 *   - manage program counter
 *   - reset and free VM
 */

/* -----------------------------------------------------------
 * Initialize VM
 * -----------------------------------------------------------
 */
int vm_init(VM *vm, size_t tape_capacity, size_t program_length)
{
    if (core_tape_init(&vm->tape, tape_capacity) != 0)
        return -1;

    if (vm_program_init(&vm->program, program_length) != 0) {
        core_tape_free(&vm->tape);
        return -2;
    }

    return 0;
}

/* -----------------------------------------------------------
 * Execute entire program
 * -----------------------------------------------------------
 */
int vm_run(VM *vm)
{
    vm_program_reset(&vm->program);

    const VMProgramStep *step;

    while ((step = vm_program_current(&vm->program)) != NULL) {

        const VMInstruction *instr = vm_get_instruction(step->opcode);
        if (!instr)
            return -1; /* unknown opcode */

        int status = instr->handler(&vm->tape, step->arg);
        if (status != 0)
            return status; /* instruction failure */

        vm_program_advance(&vm->program);
    }

    return 0;
}

/* -----------------------------------------------------------
 * Reset VM
 * -----------------------------------------------------------
 */
void vm_reset(VM *vm)
{
    core_tape_reset(&vm->tape);
    vm_program_reset(&vm->program);
}

/* -----------------------------------------------------------
 * Free VM resources
 * -----------------------------------------------------------
 */
void vm_free(VM *vm)
{
    core_tape_free(&vm->tape);
    vm_program_free(&vm->program);
}
