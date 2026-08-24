#include <stdlib.h>
#include <string.h>
#include "program.h"

/*
 * Phoenix VM Program Implementation
 *
 * Responsibilities:
 *   - allocate program steps
 *   - set (opcode, arg) pairs
 *   - manage program counter
 *   - provide current step
 *   - free resources
 */

/* -----------------------------------------------------------
 * Initialize program
 * -----------------------------------------------------------
 */
int vm_program_init(VMProgram *p, size_t length)
{
    p->steps = (VMProgramStep *)malloc(sizeof(VMProgramStep) * length);
    if (!p->steps)
        return -1;

    p->length = length;
    p->pc     = 0;

    memset(p->steps, 0, sizeof(VMProgramStep) * length);
    return 0;
}

/* -----------------------------------------------------------
 * Set a program step
 * -----------------------------------------------------------
 */
int vm_program_set(VMProgram *p, size_t index,
                   VMOpcode opcode, int arg)
{
    if (index >= p->length)
        return -1;

    p->steps[index].opcode = opcode;
    p->steps[index].arg    = arg;
    return 0;
}

/* -----------------------------------------------------------
 * Reset program counter
 * -----------------------------------------------------------
 */
void vm_program_reset(VMProgram *p)
{
    p->pc = 0;
}

/* -----------------------------------------------------------
 * Retrieve current step
 * -----------------------------------------------------------
 */
const VMProgramStep *vm_program_current(const VMProgram *p)
{
    if (p->pc >= p->length)
        return NULL;

    return &p->steps[p->pc];
}

/* -----------------------------------------------------------
 * Advance program counter
 * -----------------------------------------------------------
 */
void vm_program_advance(VMProgram *p)
{
    if (p->pc < p->length)
        p->pc++;
}

/* -----------------------------------------------------------
 * Free program resources
 * -----------------------------------------------------------
 */
void vm_program_free(VMProgram *p)
{
    free(p->steps);
    p->steps  = NULL;
    p->length = 0;
    p->pc     = 0;
}
