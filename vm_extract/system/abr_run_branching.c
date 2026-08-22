#include "abr_system.h"
#include "core_extract.c"
#include "vm_program.h"

ExecutionState abr_system_run_branching(
    ABRSystem      *sys,
    const Bitstream *S,
    const WidthSet  *G
)
{
    ExecutionState result;
    result.branches     = NULL;
    result.branch_count = 0;

    /* core: extract windows */
    WindowSet ws_core = abr_extract(S, G);

    /* pipeline: branching run */
    ExecutionState state = abr_run_branching(
        sys->pipeline,
        &ws_core,
        &sys->flags,
        &sys->ctx
    );

    if (sys->ctx.error_code != 0) {
        return state;
    }

    /* VM: execute program on each branch */
    for (size_t i = 0; i < state.branch_count; ++i) {
        state.branches[i] = abr_vm_exec(
            sys->program,
            &state.branches[i],
            &sys->flags,
            &sys->ctx
        );
        if (sys->ctx.error_code != 0) {
            break;
        }
    }

    return state;
}
