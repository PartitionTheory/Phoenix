#include "plugin.h"
#include <stdlib.h>
#include <stdio.h>

static int inc_init(Plugin *self, const Flags *flags)
{
    return 0;
}

static WindowSet inc_process(
    Plugin *self,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx)
{
    WindowSet out = *in;

    for (size_t i = 0; i < out.count; ++i) {
        for (size_t b = 0; b < out.windows[i].length; ++b) {
            /* increment bit: 0→1, 1→0 (simple toggle) */
            out.windows[i].bits[b] ^= 1;
        }
    }

    printf("[increment_plugin] toggled all bits\n");
    return out;
}


Plugin *make_increment_plugin(void)
{
    Plugin *p = calloc(1, sizeof(Plugin));
    p->init = inc_init;
    p->process_set = inc_process;
    p->is_reversible = 0;
    p->is_branching  = 0;
    return p;
}
