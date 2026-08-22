#include "flip_stretch.h"
#include <stdlib.h>

/*
A: Mathematical description
   For each window W and each bit b:
       f(b) = 1 - b
   This is a simple involution on {0,1}, but we do NOT implement reverse_set.

B: Engineering description
   We flip bits in-place. The WindowSet structure is shallow-copied.
*/

static WindowSet process_flip(
    Plugin *self,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
) {
    WindowSet out = *in;

    for (size_t i = 0; i < out.count; i++) {
        Window *w = &out.windows[i];
        for (size_t j = 0; j < w->length; j++) {
            w->bits[j] ^= 1; /* flip bit */
        }
    }

    return out;
}

Plugin *make_flip_stretch_plugin(void)
{
    Plugin *p = calloc(1, sizeof(Plugin));

    p->init           = NULL;
    p->process_set    = process_flip;
    p->reverse_set    = NULL;
    p->process_branch = NULL;

    p->is_reversible  = 0;
    p->is_branching   = 0;

    p->state = NULL;
    return p;
}

