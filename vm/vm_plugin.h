#ifndef ABR_VM_PLUGIN_H
#define ABR_VM_PLUGIN_H

#include "instruction.h"
#include "plugin.h"

/* Wrap a Plugin as an Instruction */
Instruction *abr_instruction_from_plugin(Plugin *p);

#endif /* ABR_VM_PLUGIN_H */
