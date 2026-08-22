#ifndef ABR_PIPELINE_H
#define ABR_PIPELINE_H

#include "plugin.h"
#include "windowset.h"
#include "flags.h"
#include "context.h"
#include <stddef.h>

/* Simple pipeline: ordered list of plugins */
typedef struct {
    Plugin **plugins;
    size_t   count;
} Pipeline;

/* Execution state for branching pipelines */
typedef struct {
    WindowSet *branches;
    size_t     branch_count;
} ExecutionState;

/* Initialize all plugins in the pipeline */
int abr_pipeline_init(Pipeline *pl, const Flags *flags);

/* Run linear pipeline: no branching, forward direction */
WindowSet abr_run_linear(
    const Pipeline *pl,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
);

/* Run branching pipeline: may produce multiple window sets */
ExecutionState abr_run_branching(
    const Pipeline *pl,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
);

/* Run reverse pipeline: apply reversible plugins in reverse order */
WindowSet abr_run_reverse(
    const Pipeline *pl,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
);

#endif /* ABR_PIPELINE_H */
