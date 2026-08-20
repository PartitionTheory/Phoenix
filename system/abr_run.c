#include "abr_system.h"
#include "core_extract.c"  /* or declare abr_extract in a header */
#include "vm_program.h"

WindowSet abr_system_run(
    ABRSystem      *sys,
    const Bitstream *S,
    const WidthSet  *G
)
{
    /* core: extract windows */
    WindowSet ws_core = abr_extract(S, G);

    /* pipeline: linear run */
    WindowSet ws_pipe = abr_run_linear(
        sys->pipeline,
        &ws_core,
        &sys->flags,
        &sys->ctx
    );

    if (sys->ctx.error_code != 0) {
        return ws_pipe;
    }

    /* VM: execute program */
    WindowSet ws_vm = abr_vm_exec(
        sys->program,
        &ws_pipe,
        &sys->flags,
        &sys->ctx
    );

    return ws_vm;
}
