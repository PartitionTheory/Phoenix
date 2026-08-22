#include <string.h>
#include "abr_plugin_registry.h"
#include "increment_plugin.h"
// #include "xor_plugin.h"
// #include "reverse_plugin.h"

static PluginDescriptor registry[] = {
    { "inc",  increment_plugin_create },
    // { "xor", xor_plugin_create },
    // { "rev", reverse_plugin_create },
};

Plugin *lookup_plugin(const char *name)
{
    for (size_t i = 0; i < sizeof registry / sizeof registry[0]; i++) {
        if (strcmp(name, registry[i].name) == 0)
            return registry[i].factory();
    }
    return NULL;
}
