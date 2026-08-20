#include "pipeline.h"

WindowSet abr_run_reverse(
    const Pipeline *pl,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
)
{
    WindowSet current = *in;

    /* apply reversible plugins in reverse order */
    for (size_t i = pl->count; i > 0; --i) {
        Plugin *p = pl->plugins[i - 1];
        if (!p || !p->is_reversible || !p->reverse_set) {
            continue;
        }
        current = p->reverse_set(p, &current, flags, ctx);
        if (ctx && ctx->error_code != 0) {
            break;
        }
    }

    return current;
}
