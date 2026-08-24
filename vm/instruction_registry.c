#include <stdlib.h>
#include <string.h>
#include "instruction.h"

/*
 * Phoenix VM Instruction Registry
 *
 * Responsibilities:
 *   - store instruction descriptors
 *   - map opcodes to handlers
 *   - allow dynamic registration
 *   - allow plugin injection
 *   - provide lookup for VM execution
 */

static VMInstruction registry[VM_MAX_OPCODE];

/* -----------------------------------------------------------
 * Register instruction
 * -----------------------------------------------------------
 */
int vm_register_instruction(VMOpcode opcode,
                            const char *name,
                            VMInstructionHandler handler)
{
    if (opcode <= VM_NOP || opcode >= VM_MAX_OPCODE)
        return -1;

    registry[opcode].opcode  = opcode;
    registry[opcode].name    = name;
    registry[opcode].handler = handler;

    return 0;
}

/* -----------------------------------------------------------
 * Retrieve instruction descriptor
 * -----------------------------------------------------------
 */
const VMInstruction *vm_get_instruction(VMOpcode opcode)
{
    if (opcode <= VM_NOP || opcode >= VM_MAX_OPCODE)
        return NULL;

    return &registry[opcode];
}

/* -----------------------------------------------------------
 * Static initialization of built‑in instructions
 * -----------------------------------------------------------
 *
 * These are the primitive instructions that form the base VM.
 * Plugins may register additional instructions later.
 */

static int builtin_write(CoreTape *t, int arg)
{
    return core_tape_write(t, arg);
}

static int builtin_move_right(CoreTape *t, int arg)
{
    (void)arg;
    return core_tape_move_right(t);
}

static int builtin_move_left(CoreTape *t, int arg)
{
    (void)arg;
    return core_tape_move_left(t);
}


static int builtin_add(CoreTape *t, int arg)
{
    int v;
    if (core_tape_read(t, &v) != 0)
        return -1;
    return core_tape_write(t, v + arg);
}

static int builtin_sub(CoreTape *t, int arg)
{
    int v;
    if (core_tape_read(t, &v) != 0)
        return -1;
    return core_tape_write(t, v - arg);
}

/* -----------------------------------------------------------
 * Initialize registry with built‑in instructions
 * -----------------------------------------------------------
 */
__attribute__((constructor))
static void vm_init_registry(void)
{
    vm_register_instruction(VM_WRITE,      "write",      builtin_write);
    vm_register_instruction(VM_MOVE_RIGHT, "move_right", builtin_move_right);
    vm_register_instruction(VM_MOVE_LEFT,  "move_left",  builtin_move_left);
    vm_register_instruction(VM_ADD,        "add",        builtin_add);
    vm_register_instruction(VM_SUB,        "sub",        builtin_sub);
}
