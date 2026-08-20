#include "vm_plugin.h"
#include <stdlib.h>

/* Forward exec wrapper */
static WindowSet instr_exec(
    Instruction      *self,
    const WindowSet  *in,
    const Flags      *flags,
    Context          *ctx
)
{
    Plugin *p = (Plugin *)self->state;
    return p->process_set(p, in, flags, ctx);
}

/* Reverse exec wrapper */
static WindowSet instr_reverse(
    Instruction      *self,
    const WindowSet  *in,
    const Flags      *flags,
    Context          *ctx
)
{
    Plugin *p = (Plugin *)self->state;
    return p->reverse_set(p, in, flags, ctx);
}

/* Branching exec wrapper */
static WindowSet *instr_exec_branch(
    Instruction      *self,
    const WindowSet  *in,
    const Flags      *flags,
    Context          *ctx,
    size_t           *out_count
)
{
    Plugin *p = (Plugin *)self->state;
    return p->process_branch(p, in, flags, ctx, out_count);
}

/* Construct an Instruction from a Plugin */
Instruction *abr_instruction_from_plugin(Plugin *p)
{
    Instruction *I = (Instruction *)calloc(1, sizeof(Instruction));
    if (!I) return NULL;

    I->state = p;

    I->exec = p->process_set ? instr_exec : NULL;

    I->reverse = (p->is_reversible && p->reverse_set)
                 ? instr_reverse
                 : NULL;

    I->exec_branch = (p->is_branching && p->process_branch)
                     ? instr_exec_branch
                     : NULL;

    I->is_reversible = p->is_reversible;
    I->is_branching  = p->is_branching;

    return I;
}
