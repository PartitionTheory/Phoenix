#include "../../include/abr_vm.h"

/* Phoenix Structural Machine — VM Engine
 *
 * This file implements the Epoch‑1 execution chamber for Phoenix.
 * It provides deterministic step‑execution and program‑execution.
 * No branching, no plugins, no advanced temporal logic.
 */

phoenix_bool phoenix_vm_step(phoenix_vm_context *ctx,
                             const phoenix_instruction *instr)
{
    if (ctx == NULL || instr == NULL)
        return PHOENIX_FALSE;

    if (ctx->halted == PHOENIX_TRUE)
        return PHOENIX_FALSE;

    /* Epoch‑1 rule:
     * accumulator = accumulator + operand
     * opcode is ignored in Epoch‑1
     */
    ctx->accumulator += instr->operand;

    return PHOENIX_TRUE;
}

phoenix_bool phoenix_vm_run(phoenix_vm_context *ctx,
                            const phoenix_instruction *program,
                            phoenix_int64 length)
{
    if (ctx == NULL || program == NULL || length <= 0)
        return PHOENIX_FALSE;

    for (phoenix_int64 i = 0; i < length; i++)
    {
        if (!phoenix_vm_step(ctx, &program[i]))
            return PHOENIX_FALSE;
    }

    return PHOENIX_TRUE;
}

