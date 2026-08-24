#include "system_modes.h"
#include "system.h"
#include "../vm/vm_plugin.h"

/*
 * Apply a system mode.
 *
 * This configures plugin behavior and any mode-specific flags.
 */

int system_mode_apply(PhoenixSystem *sys, SystemMode mode)
{
    switch (mode) {

        case SYSTEM_MODE_NORMAL:
            /* No plugins */
            sys->plugins_loaded = 0;
            return 0;

        case SYSTEM_MODE_PLUGIN:
            /* Auto-load plugins */
            if (system_load_plugins(sys) != 0)
                return -1;
            return 0;

        case SYSTEM_MODE_SAFE:
            /* Plugins disabled */
            sys->plugins_loaded = 0;
            return 0;

        case SYSTEM_MODE_DIAGNOSTIC:
            /* Plugins allowed; logging would occur here */
            sys->plugins_loaded = 0; /* plugins load only when requested */
            return 0;

        default:
            return -1;
    }
}
