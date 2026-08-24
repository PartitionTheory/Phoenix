#include "abr_plugin_registry.h"
#include "increment_plugin.h"
#include "flip_stretch.h"
#include "branch_merge_split.h"
#include <string.h>

/*
A: Mathematical description
   abr_load_plugin(name) returns a Plugin* corresponding to the
   transform T_name.

B: Engineering description
   Simple string-based registry.
   - No dynamic loading
   - No shared libraries
   - Pure static dispatch
*/

Plugin *abr_load_plugin(const char *name)
{
    if (strcmp(name, "increment") == 0) {
        return make_increment_plugin();
    }

    if (strcmp(name, "flip_stretch") == 0) {
        return make_flip_stretch_plugin();
    }

    if (strcmp(name, "branch_merge_split") == 0) {
        return make_branch_merge_split_plugin();
    }

    return NULL; /* unknown plugin */
}

