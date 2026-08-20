#include "vm_program.h"

WindowSet abr_vm_exec(
    const VMProgram *prog,
    const WindowSet *in,
    const Flags     *flags,
    Context         *ctx
)
{
    WindowSet current = *in;

    for (size_t i = 0; i < prog->count; ++i) {
        Instruction *I = prog->instructions[i];
        if (!I || !I->exec) continue;

        current = I->exec(I, &current, flags, ctx);
        if (ctx && ctx->error_code != 0) break;
    }

    return current;
}

WindowSet abr_vm_exec_reverse(
    const VMProgram *prog,
    const WindowSet *in,
    const Flags     *flags,
    Context         *ctx
)
{
    WindowSet current = *in;

    for (size_t i = prog->count; i > 0; --i) {
        Instruction *I = prog->instructions[i - 1];
        if (!I || !I->is_reversible || !I->reverse) continue;

        current = I->reverse(I, &current, flags, ctx);
        if (ctx && ctx->error_code != 0) break;
    }

    return current;
}
