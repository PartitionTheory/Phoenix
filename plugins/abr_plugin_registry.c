#include "abr_plugin_registry.h"
#include "increment_plugin.h"
#include "flip_stretch.h"
#include "branch_merge_split.h"

/*
 * Built-in Phoenix plugins
 *
 * Each plugin exposes a descriptor with a Phoenix-compatible apply()
 * taking (struct abr_context *, BitWindow *).
 */

static const abr_plugin_descriptor increment_plugin_desc = {
    .name        = "increment",
    .description = "Increment bits in window",
    .apply       = increment_plugin_apply
};

static const abr_plugin_descriptor flip_stretch_plugin_desc = {
    .name        = "flip_stretch",
    .description = "Flip and stretch window",
    .apply       = flip_stretch_plugin_apply
};

static const abr_plugin_descriptor branch_merge_split_plugin_desc = {
    .name        = "branch_merge_split",
    .description = "Branch, merge, and split window",
    .apply       = branch_merge_split_plugin_apply
};

static const abr_plugin_entry PLUGINS[] = {
    { &increment_plugin_desc },
    { &flip_stretch_plugin_desc },
    { &branch_merge_split_plugin_desc }
};

static const abr_plugin_registry GLOBAL_REGISTRY = {
    .entries = PLUGINS,
    .count   = (int)(sizeof(PLUGINS) / sizeof(PLUGINS[0]))
};

const abr_plugin_registry *abr_get_plugin_registry(void)
{
    return &GLOBAL_REGISTRY;
}

const abr_plugin_entry *abr_find_plugin(const char *name)
{
    if (!name) return NULL;

    for (int i = 0; i < GLOBAL_REGISTRY.count; ++i) {
        const abr_plugin_descriptor *d = GLOBAL_REGISTRY.entries[i].desc;
        if (d && d->name && strcmp(d->name, name) == 0) {
            return &GLOBAL_REGISTRY.entries[i];
        }
    }
    return NULL;
}
