#ifndef ABR_SYSTEM_H
#define ABR_SYSTEM_H

#include "bitstream.h"
#include "widthset.h"
#include "windowset.h"
#include "pipeline.h"
#include "vm_program.h"
#include "flags.h"
#include "context.h"

/* Top-level ABR system */
typedef struct {
    Pipeline  *pipeline;   /* plugin pipeline */
    VMProgram *program;    /* VM program */
    Flags      flags;      /* global flags */
    Context    ctx;        /* global context */
} ABRSystem;

/* Initialize ABR system (pipeline, program, flags, context) */
int abr_system_init(ABRSystem *sys, Pipeline *pl, VMProgram *prog, const Flags *flags);

/* Linear run: core -> pipeline -> VM */
WindowSet abr_system_run(
    ABRSystem      *sys,
    const Bitstream *S,
    const WidthSet  *G
);

/* Branching run: core -> pipeline (branching) -> VM per branch */
ExecutionState abr_system_run_branching(
    ABRSystem      *sys,
    const Bitstream *S,
    const WidthSet  *G
);

/* Reverse run: VM reverse -> pipeline reverse (no core) */
WindowSet abr_system_run_reverse(
    ABRSystem      *sys,
    const WindowSet *final_ws
);

#endif /* ABR_SYSTEM_H */
