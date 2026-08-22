#ifndef ABR_VM_PROGRAM_H
#define ABR_VM_PROGRAM_H

#include "instruction.h"
#include <stddef.h>

/* A VM program is an ordered list of instructions */
typedef struct {
    Instruction **instructions;
    size_t        count;
} VMProgram;

/* Execute program forward */
WindowSet abr_vm_exec(
    const VMProgram *prog,
    const WindowSet *in,
    const Flags     *flags,
    Context         *ctx
);

/* Execute program in reverse (only reversible instructions) */
WindowSet abr_vm_exec_reverse(
    const VMProgram *prog,
    const WindowSet *in,
    const Flags     *flags,
    Context         *ctx
);

#endif /* ABR_VM_PROGRAM_H */
