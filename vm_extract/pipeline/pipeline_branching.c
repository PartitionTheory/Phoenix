#include "pipeline.h"
#include <stdlib.h>

ExecutionState abr_run_branching(
    const Pipeline *pl,
    const WindowSet *in,
    const Flags *flags,
    Context *ctx
)
{
    ExecutionState state;
    state.branches     = NULL;
    state.branch_count = 0;

    /* start with a single branch: the input */
    WindowSet *current_branches =
        (WindowSet *)malloc(sizeof(WindowSet));
    if (!current_branches) {
        return state;
    }
    current_branches[0] = *in;
    size_t current_count = 1;

    for (size_t i = 0; i < pl->count; ++i) {
        Plugin *p = pl->plugins[i];
        if (!p) {
            continue;
        }

        if (p->is_branching && p->process_branch) {
            /* branching plugin: expand branches */
            WindowSet *next_branches = NULL;
            size_t     next_count    = 0;

            for (size_t b = 0; b < current_count; ++b) {
                size_t out_count = 0;
                WindowSet *out =
                    p->process_branch(p, &current_branches[b],
                                      flags, ctx, &out_count);
                if (ctx && ctx->error_code != 0) {
                    break;
                }

                if (out && out_count > 0) {
                    WindowSet *tmp = (WindowSet *)realloc(
                        next_branches,
                        (next_count + out_count) * sizeof(WindowSet)
                    );
                    if (!tmp) {
                        break;
                    }
                    next_branches = tmp;
                    for (size_t k = 0; k < out_count; ++k) {
                        next_branches[next_count + k] = out[k];
                    }
                    next_count += out_count;
                }
            }

            free(current_branches);
            current_branches = next_branches;
            current_count    = next_count;
        } else if (p->process_set) {
            /* non-branching plugin: apply to each branch */
            for (size_t b = 0; b < current_count; ++b) {
                current_branches[b] =
                    p->process_set(p, &current_branches[b],
                                   flags, ctx);
                if (ctx && ctx->error_code != 0) {
                    break;
                }
            }
        }

        if (ctx && ctx->error_code != 0) {
            break;
        }
    }

    state.branches     = current_branches;
    state.branch_count = current_count;
    return state;
}
