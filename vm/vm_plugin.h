#ifndef PHOENIX_VM_PLUGIN_H
#define PHOENIX_VM_PLUGIN_H

#include "instruction.h"

/*
 * Phoenix VM Plugin Interface
 *
 * Plugins may:
 *   - register new instructions
 *   - override existing instructions
 *   - extend VM behavior
 *
 * Formal model:
 *   A plugin is a module that calls:
 *
 *       vm_register_instruction(opcode, name, handler)
 *
 * during initialization.
 */

/* -----------------------------------------------------------
 * Plugin initialization function signature
 * -----------------------------------------------------------
 *
 * Each plugin must expose a function with this signature:
 *
 *   int vm_plugin_init(void);
 *
 * Return:
 *   0 on success
 *   nonzero on failure
 */
typedef int (*VMPluginInitFunc)(void);

/* -----------------------------------------------------------
 * Plugin registration API
 * -----------------------------------------------------------
 */

/* Register a plugin initialization function */
int vm_plugin_register(VMPluginInitFunc init_func);

/* Run all registered plugin initializers */
int vm_plugin_run_all(void);

#endif /* PHOENIX_VM_PLUGIN_H */
