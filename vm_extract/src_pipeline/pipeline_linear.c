#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pipeline.h"
#include "pipeline_init.h"
#include "abr_plugin_registry.h"

/*
 * Linear pipeline execution:
 *
 *     W₀ --p--> W₁
 *
 * A single plugin is applied to the entire WindowSet.
 */
abr_windowset pipeline_run_linear(pipeline_context *ctx, abr_windowset ws)
{
    if (!ctx || !ctx->plugin_name) {
        fprintf(stderr, "pipeline_run_linear: invalid context or plugin name\n");
        return ws;
    }

    // Lookup plugin from registry
    const abr_plugin_descriptor *plugin = lookup_plugin(ctx->plugin_name);
    if (!plugin) {
        fprintf(stderr, "Unknown pipeline plugin: %s\n", ctx->plugin_name);
        return ws;  // or exit(1) depending on your style
    }

    // Apply plugin morphism
    return plugin->apply(ws);
}

