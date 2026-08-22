#ifndef ABR_PLUGIN_H
#define ABR_PLUGIN_H

#include "windowset.h"
#include "flags.h"
#include "context.h"
#include <stddef.h>

typedef struct Plugin Plugin;

struct Plugin {
    int (*init)(Plugin *self, const Flags *flags);

    WindowSet (*process_set)(
        Plugin *self,
        const WindowSet *in,
        const Flags *flags,
        Context *ctx
    );

    WindowSet (*reverse_set)(
        Plugin *self,
        const WindowSet *in,
        const Flags *flags,
        Context *ctx
    );

    WindowSet *(*process_branch)(
        Plugin *self,
        const WindowSet *in,
        const Flags *flags,
        Context *ctx,
        size_t *out_count
    );

    int is_reversible;
    int is_branching;

    void *state;
};

#endif

