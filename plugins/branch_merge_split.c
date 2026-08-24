#include "branch_merge_split.h"
#include <stdlib.h>
#include <string.h>

/*
A: Mathematical description
   Input: WindowSet W with windows W0, W1, W2, W3.
   Output:
       O0 = concat(W0, W1)
       O1 = first 3 bits of W2
       O2 = next 3 bits of W2
   Output count = 3.

B: Engineering description
   - Allocate new WindowSet
   - Allocate new windows
   - Copy bit ranges explicitly
   - No reverse_set, no process_set
*/

static WindowSet *process_branch_merge_split(
    Plugin *self,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx,
    size_t *out_count
) {
    /* Guard: need at least 3 windows and at least 6 bits in W2 */
    if (in->count < 3 || in->windows[2].length < 6) {
        *out_count = 0;

        WindowSet *out = calloc(1, sizeof(WindowSet));
        out->count = 0;
        out->windows = NULL;
        return out;
    }

    *out_count = 3;

    WindowSet *out = calloc(1, sizeof(WindowSet));
    out->count = 3;
    out->windows = calloc(3, sizeof(Window));

    /* Merge W0 + W1 */
    const Window *w0 = &in->windows[0];
    const Window *w1 = &in->windows[1];

    size_t merged_len = w0->length + w1->length;

    out->windows[0].width  = merged_len;
    out->windows[0].length = merged_len;
    out->windows[0].bits   = malloc(merged_len);

    memcpy(out->windows[0].bits,              w0->bits, w0->length);
    memcpy(out->windows[0].bits + w0->length, w1->bits, w1->length);

    /* Split W2 into two 3-bit windows */
    const Window *w2 = &in->windows[2];

    out->windows[1].width  = 3;
    out->windows[1].length = 3;
    out->windows[1].bits   = malloc(3);
    memcpy(out->windows[1].bits, w2->bits, 3);

    out->windows[2].width  = 3;
    out->windows[2].length = 3;
    out->windows[2].bits   = malloc(3);
    memcpy(out->windows[2].bits, w2->bits + 3, 3);

    return out;
}

Plugin *make_branch_merge_split_plugin(void)
{
    Plugin *p = calloc(1, sizeof(Plugin));

    p->init           = NULL;
    p->process_set    = NULL;  /* non-branching path unused */
    p->reverse_set    = NULL;

    p->process_branch = process_branch_merge_split;

    p->is_reversible  = 0;
    p->is_branching   = 1;

    p->state = NULL;
    return p;
}
