#ifndef ABR_PLUGIN_H
#define ABR_PLUGIN_H

#include "abr_window.h"

/*
 * A plugin is a pure morphism:
 *
 *     p : WindowSet → WindowSet
 *
 * It takes an abr_windowset and returns a transformed abr_windowset.
 */
typedef abr_windowset (*abr_plugin_fn)(abr_windowset ws);

#endif
