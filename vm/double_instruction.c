#include "instruction.h"
#include <stdlib.h>
#include <stdio.h>

static WindowSet double_exec(
    Instruction *self,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx)
{
    WindowSet out = *in;

    for (size_t i = 0; i < out.count; ++i) {
        out.windows[i].value *= 2;
    }

    printf("[double_instruction] doubled all windows\n");
    return out;
}

Instruction *make_double_instruction(void)
{
    Instruction *I = calloc(1, sizeof(Instruction));
    I->exec = double_exec;
    I->is_reversible = 0;
    I->is_branching  = 0;
    return I;
}
