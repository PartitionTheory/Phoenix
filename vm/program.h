#ifndef PHOENIX_VM_PROGRAM_H
#define PHOENIX_VM_PROGRAM_H

#include <stddef.h>
#include "instruction.h"

/*
 * Phoenix VM Program
 *
 * Formal model:
 *   A program is a sequence of (opcode, arg) pairs:
 *
 *       P = [(op_0, a_0), (op_1, a_1), ..., (op_n, a_n)]
 *
 *   Execution model:
 *       - program counter (pc) starts at 0
 *       - each step executes one instruction
 *       - pc increments after each instruction
 *       - execution stops when pc == length
 */

typedef struct {
    VMOpcode opcode;
    int      arg;
} VMProgramStep;

typedef struct {
    VMProgramStep *steps;   /* array of steps */
    size_t         length;  /* number of steps */
    size_t         pc;      /* program counter */
} VMProgram;

/* -----------------------------------------------------------
 * Program API
 * -----------------------------------------------------------
 */

/* Initialize program with given number of steps */
int vm_program_init(VMProgram *p, size_t length);

/* Set a step: (opcode, arg) */
int vm_program_set(VMProgram *p, size_t index,
                   VMOpcode opcode, int arg);

/* Reset program counter to 0 */
void vm_program_reset(VMProgram *p);

/* Retrieve current step (NULL if pc >= length) */
const VMProgramStep *vm_program_current(const VMProgram *p);

/* Advance program counter */
void vm_program_advance(VMProgram *p);

/* Free program resources */
void vm_program_free(VMProgram *p);

#endif /* PHOENIX_VM_PROGRAM_H */
