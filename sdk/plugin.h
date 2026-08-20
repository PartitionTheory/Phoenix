#ifndef ABR_PLUGIN_H
#define ABR_PLUGIN_H

#include "windowset.h"
#include "flags.h"
#include "context.h"

/* Pure-C plugin interface: function pointer table */

typedef struct Plugin Plugin;

struct Plugin {
    /* Initialize plugin with flags; return 0 on success */
    int (*init)(Plugin *self, const Flags *flags);

    /* Process a single window set (non-branching) */
    WindowSet (*process_set)(
        Plugin       *self,
        const WindowSet *in,
        const Flags  *flags,
        Context      *ctx
    );

    /* Optional reversible transform; may be NULL if not reversible */
    WindowSet (*reverse_set)(
        Plugin       *self,
        const WindowSet *in,
        const Flags  *flags,
        Context      *ctx
    );

    /* Optional branching transform; returns array of WindowSet.
       If NULL, plugin is non-branching. */
    WindowSet *(*process_branch)(
        Plugin       *self,
        const WindowSet *in,
        const Flags  *flags,
        Context      *ctx,
        size_t       *out_count
    );

    /* Capability flags */
    int is_reversible; /* 1 if reverse_set is meaningful */
    int is_branching;  /* 1 if process_branch is meaningful */

    /* Optional plugin-specific state */
    void *state;
};

#endif /* ABR_PLUGIN_H */
