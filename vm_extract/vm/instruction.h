#ifndef ABR_INSTRUCTION_H
#define ABR_INSTRUCTION_H

#include "windowset.h"
#include "flags.h"
#include "context.h"
#include <stddef.h>

/* Pure-C instruction interface */
typedef struct Instruction Instruction;

struct Instruction {
    /* Forward execution */
    WindowSet (*exec)(
        Instruction      *self,
        const WindowSet  *in,
        const Flags      *flags,
        Context          *ctx
    );

    /* Optional reverse execution */
    WindowSet (*reverse)(
        Instruction      *self,
        const WindowSet  *in,
        const Flags      *flags,
        Context          *ctx
    );

    /* Optional branching execution */
    WindowSet *(*exec_branch)(
        Instruction      *self,
        const WindowSet  *in,
        const Flags      *flags,
        Context          *ctx,
        size_t           *out_count
    );

    int is_reversible;
    int is_branching;

    void *state; /* optional instruction-specific state */
};

#endif /* ABR_INSTRUCTION_H */
