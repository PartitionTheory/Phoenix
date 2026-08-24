#ifndef ABR_PLUGIN_API_H
#define ABR_PLUGIN_API_H

#include <stddef.h>
#include <stdint.h>
#include "bigint.h"

/*
 * ABR Plugin API (Phoenix Edition)
 *
 * This header defines the ABI boundary between:
 *   - ABR core system
 *   - dynamically loaded plugins
 *
 * Plugins operate on BigInt values and produce BigInt outputs.
 * The system guarantees:
 *   - BigInt is a fully defined struct (no forward-decl conflicts)
 *   - Memory ownership rules are explicit
 *   - All plugin calls are pure functions (no hidden global state)
 *
 * Math / Engineering view:
 *   A plugin is a function:
 *
 *       f : BigInt → BigInt
 *
 *   with the constraint that:
 *       |f(x)| ≤ MAX_BITS
 *
 *   enforced by the system.
 *
 *   Plugins may also expose metadata describing their domain,
 *   codomain, and invariants.
 */

/* -----------------------------------------------------------
 * Plugin metadata
 * -----------------------------------------------------------
 *
 * name:
 *   Human-readable identifier.
 *
 * version:
 *   Semantic versioning (major.minor.patch).
 *
 * description:
 *   Optional text describing the plugin's behavior.
 *
 * Math view:
 *   Metadata is not part of the function f itself, but part of
 *   the "certificate" that accompanies f. It allows the system
 *   to reason about plugin composition and invariants.
 */
typedef struct {
    const char *name;
    const char *version;
    const char *description;
} AbrPluginInfo;

/* -----------------------------------------------------------
 * Plugin function type
 * -----------------------------------------------------------
 *
 * A plugin receives:
 *   - const BigInt *input
 *   - BigInt *output (preallocated by system)
 *
 * It must:
 *   - write its result into output->bytes
 *   - set output->byte_len and output->bit_len
 *
 * It must NOT:
 *   - allocate new BigInt objects
 *   - free the input or output
 *
 * Math view:
 *   f(x) is computed in-place into the output buffer.
 */
typedef int (*AbrPluginFn)(const BigInt *input, BigInt *output);

/* -----------------------------------------------------------
 * Plugin descriptor
 * -----------------------------------------------------------
 *
 * This is the object the system loads from each plugin.
 *
 * info:
 *   Metadata describing the plugin.
 *
 * run:
 *   The plugin's function f.
 *
 * Math view:
 *   A plugin is a pair (info, f).
 */
typedef struct {
    AbrPluginInfo info;
    AbrPluginFn   run;
} AbrPlugin;

/* -----------------------------------------------------------
 * Plugin registration entry point
 * -----------------------------------------------------------
 *
 * Every plugin must export:
 *
 *     AbrPlugin* abr_plugin_init(void);
 *
 * The system calls this once when loading the plugin.
 *
 * Math view:
 *   abr_plugin_init returns the pair (info, f).
 */
AbrPlugin *abr_plugin_init(void);

#endif /* ABR_PLUGIN_API_H */
