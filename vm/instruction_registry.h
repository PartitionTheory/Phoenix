#ifndef ABR_INSTRUCTION_REGISTRY_H
#define ABR_INSTRUCTION_REGISTRY_H

#include "instruction.h"

typedef Instruction *(*InstructionFactory)(void);

typedef struct {
    const char *name;
    InstructionFactory factory;
} InstructionEntry;

Instruction *instruction_registry_lookup(const char *name);
void instruction_registry_list(void);

#endif
