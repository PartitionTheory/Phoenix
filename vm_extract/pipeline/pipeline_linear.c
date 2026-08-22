#include "pipeline.h"

WindowSet abr_run_linear(
    const Pipeline *pl,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
)
{
    WindowSet current = *in;

    for (size_t i = 0; i < pl->count; ++i) {
        Plugin *p = pl->plugins[i];
        if (!p || !p->process_set) {
            continue;
        }
        current = p->process_set(p, &current, flags, ctx);
        if (ctx && ctx->error_code != 0) {
            break;
        }
    }

    return current;
}
