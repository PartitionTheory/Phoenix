#ifndef PHOENIX_SYSTEM_H
#define PHOENIX_SYSTEM_H

#include <stddef.h>
#include "../vm/vm.h"
#include "../vm/vm_plugin.h"

/*
 * Phoenix System Layer
 *
 * Responsibilities:
 *   - initialize VM
 *   - load plugins
 *   - load program
 *   - execute program
 *   - provide high-level system API
 *
 * This is the top-level interface used by the CLI and
 * any external embedding of Phoenix.
 */

typedef struct {
    VM vm;           /* underlying VM */
    int plugins_loaded;
} PhoenixSystem;

/* -----------------------------------------------------------
 * System API
 * -----------------------------------------------------------
 */

/* Initialize system with tape capacity and program length */
int system_init(PhoenixSystem *sys,
                size_t tape_capacity,
                size_t program_length);

/* Load all registered plugins */
int system_load_plugins(PhoenixSystem *sys);

/* Load a program step-by-step */
int system_load_step(PhoenixSystem *sys,
                     size_t index,
                     VMOpcode opcode,
                     int arg);

/* Execute the loaded program */
int system_run(PhoenixSystem *sys);

/* Reset system (VM + plugins flag) */
void system_reset(PhoenixSystem *sys);

/* Free all system resources */
void system_free(PhoenixSystem *sys);

#endif /* PHOENIX_SYSTEM_H */
