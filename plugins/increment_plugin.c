#include "increment_plugin.h"
#include <stdlib.h>

/*
A: Mathematical description
   Identity transform:
   For any WindowSet W, process_set(W) = W.

B: Engineering description
   We return a shallow copy of the WindowSet structure.
   (The engine owns the memory; this plugin does not allocate.)
*/

static WindowSet process_increment(
    Plugin *self,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
) {
    return *in; /* ABI-safe shallow copy */
}

Plugin *make_increment_plugin(void)
{
    Plugin *p = calloc(1, sizeof(Plugin));

    p->init           = NULL;
    p->process_set    = process_increment;
    p->reverse_set    = NULL;
    p->process_branch = NULL;

    p->is_reversible  = 0;
    p->is_branching   = 0;

    p->state = NULL;
    return p;
}

