#ifndef PHOENIX_VM_H
#define PHOENIX_VM_H

#include "../core/interface/core_tape.h"
#include "program.h"
#include "instruction.h"

/*
 * Phoenix VM Execution Engine
 *
 * Formal model:
 *   VM executes a program P over a tape T.
 *
 *   Execution:
 *       while (step = current(P)):
 *           execute(step.opcode, step.arg)
 *           advance(P)
 *
 *   Result:
 *       0 on success
 *       nonzero on failure
 */

typedef struct {
    CoreTape tape;     /* execution tape */
    VMProgram program; /* program to execute */
} VM;

/* -----------------------------------------------------------
 * VM API
 * -----------------------------------------------------------
 */

/* Initialize VM with tape capacity and program length */
int vm_init(VM *vm, size_t tape_capacity, size_t program_length);

/* Execute entire program */
int vm_run(VM *vm);

/* Reset VM (tape + program counter) */
void vm_reset(VM *vm);

/* Free VM resources */
void vm_free(VM *vm);

#endif /* PHOENIX_VM_H */
