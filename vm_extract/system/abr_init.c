#include "abr_system.h"
#include "pipeline.h"

int abr_system_init(ABRSystem *sys, Pipeline *pl, VMProgram *prog, const Flags *flags)
{
    if (!sys || !pl || !prog) return -1;

    sys->pipeline = pl;
    sys->program  = prog;

    sys->flags = *flags;
    sys->ctx.branch_id  = 0;
    sys->ctx.error_code = 0;
    sys->ctx.user_data  = NULL;

    /* initialize pipeline plugins */
    int rc = abr_pipeline_init(pl, flags);
    if (rc != 0) {
        return rc;
    }

    return 0;
}
