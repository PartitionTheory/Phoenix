#include "abr_system.h"
#include "pipeline.h"
#include "vm_program.h"

WindowSet abr_system_run_reverse(
    ABRSystem      *sys,
    const WindowSet *final_ws
)
{
    /* VM: reverse program */
    WindowSet ws_vm_rev = abr_vm_exec_reverse(
        sys->program,
        final_ws,
        &sys->flags,
        &sys->ctx
    );

    if (sys->ctx.error_code != 0) {
        return ws_vm_rev;
    }

    /* pipeline: reverse */
    WindowSet ws_pipe_rev = abr_run_reverse(
        sys->pipeline,
        &ws_vm_rev,
        &sys->flags,
        &sys->ctx
    );

    return ws_pipe_rev;
}
