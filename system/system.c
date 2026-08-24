#include "system.h"

/*
 * Phoenix System Layer Implementation
 *
 * Responsibilities:
 *   - initialize VM
 *   - load plugins
 *   - load program
 *   - execute program
 *   - manage lifecycle
 */

/* -----------------------------------------------------------
 * Initialize system
 * -----------------------------------------------------------
 */
int system_init(PhoenixSystem *sys,
                size_t tape_capacity,
                size_t program_length)
{
    sys->plugins_loaded = 0;

    if (vm_init(&sys->vm, tape_capacity, program_length) != 0)
        return -1;

    return 0;
}

/* -----------------------------------------------------------
 * Load all registered plugins
 * -----------------------------------------------------------
 */
int system_load_plugins(PhoenixSystem *sys)
{
    int status = vm_plugin_run_all();
    if (status != 0)
        return status;

    sys->plugins_loaded = 1;
    return 0;
}

/* -----------------------------------------------------------
 * Load a program step
 * -----------------------------------------------------------
 */
int system_load_step(PhoenixSystem *sys,
                     size_t index,
                     VMOpcode opcode,
                     int arg)
{
    return vm_program_set(&sys->vm.program, index, opcode, arg);
}

/* -----------------------------------------------------------
 * Execute program
 * -----------------------------------------------------------
 */
int system_run(PhoenixSystem *sys)
{
    return vm_run(&sys->vm);
}

/* -----------------------------------------------------------
 * Reset system
 * -----------------------------------------------------------
 */
void system_reset(PhoenixSystem *sys)
{
    vm_reset(&sys->vm);
    sys->plugins_loaded = 0;
}

/* -----------------------------------------------------------
 * Free system resources
 * -----------------------------------------------------------
 */
void system_free(PhoenixSystem *sys)
{
    vm_free(&sys->vm);
    sys->plugins_loaded = 0;
}
