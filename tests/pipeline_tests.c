#include <stdio.h>
#include <stdlib.h>
#include "pipeline.h"
#include "plugin.h"
#include "windowset.h"
#include "flags.h"
#include "context.h"

/* Dummy plugin */
static WindowSet dummy_process(
    Plugin *self, const WindowSet *in, const Flags *flags, Context *ctx)
{
    printf("pipeline_tests: plugin exec\n");
    return *in;
}

int main(void)
{
    Plugin *p = calloc(1, sizeof(Plugin));
    p->process_set = dummy_process;

    Pipeline pl;
    pl.plugins = calloc(1, sizeof(Plugin *));
    pl.plugins[0] = p;
    pl.count = 1;

    WindowSet ws = {0};
    Flags flags = {0};
    Context ctx = {0};

    WindowSet out = abr_run_linear(&pl, &ws, &flags, &ctx);

    printf("pipeline_tests: ok\n");
    return 0;
}
