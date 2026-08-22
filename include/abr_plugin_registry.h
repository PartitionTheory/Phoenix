#ifndef ABR_PLUGIN_REGISTRY_H
#define ABR_PLUGIN_REGISTRY_H

#include "plugin.h"        // defines Plugin
#include "window.h"        // defines Window, WindowSet
#include "pipeline.h"      // defines PipelineContext

// Registry entry
typedef struct {
    const char *name;
    Plugin *(*factory)(void);   // plugin constructor
} PluginDescriptor;

// Lookup plugin by name
Plugin *lookup_plugin(const char *name);

#endif
