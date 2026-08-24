#ifndef PHOENIX_SYSTEM_MODES_H
#define PHOENIX_SYSTEM_MODES_H

#include "system.h"

/*
 * Phoenix System Modes
 *
 * These define high-level execution behaviors:
 *   - NORMAL: no plugins
 *   - PLUGIN: auto-load plugins
 *   - SAFE: plugins disabled
 *   - DIAGNOSTIC: plugins allowed + logging
 */

typedef enum {
    SYSTEM_MODE_NORMAL = 0,
    SYSTEM_MODE_PLUGIN = 1,
    SYSTEM_MODE_SAFE = 2,
    SYSTEM_MODE_DIAGNOSTIC = 3
} SystemMode;

/* Apply a mode to the system */
int system_mode_apply(PhoenixSystem *sys, SystemMode mode);

#endif /* PHOENIX_SYSTEM_MODES_H */
