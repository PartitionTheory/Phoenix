#include <stdio.h>
#include <string.h>

#include "instruction_registry.h"
#include "double_instruction.h"   // add more as you create them

static InstructionEntry registry[] = {
    { "double", make_double_instruction },
    // future instructions:
    // { "rev", make_reverse_instruction },
    // { "branch", make_branch_instruction },
};

static const size_t registry_count = sizeof(registry) / sizeof(registry[0]);

Instruction *instruction_registry_lookup(const char *name)
{
    for (size_t i = 0; i < registry_count; ++i) {
        if (strcmp(registry[i].name, name) == 0) {
            return registry[i].factory();
        }
    }
    return NULL;
}

void instruction_registry_list(void)
{
    printf("Available VM instructions:\n");
    for (size_t i = 0; i < registry_count; ++i) {
        printf("  %s\n", registry[i].name);
    }
}
