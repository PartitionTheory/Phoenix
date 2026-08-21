#include "double_instruction.h"
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
        for (size_t b = 0; b < out.windows[i].length; ++b) {
            out.windows[i].bits[b] = (out.windows[i].bits[b] == 0 ? 0 : 1);
        }
    }

    printf("[double_instruction] (placeholder op)\n");
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
