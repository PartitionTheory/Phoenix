#ifndef PHOENIX_VM_INSTRUCTION_H
#define PHOENIX_VM_INSTRUCTION_H

#include <stddef.h>
#include "../core/interface/core_tape.h"

/*
 * Phoenix VM Instruction System
 *
 * Formal model:
 *   An instruction is a function:
 *
 *       f : (Tape, arg) -> status
 *
 *   where:
 *       - Tape is the core tape
 *       - arg is an integer argument
 *       - status is 0 on success, nonzero on failure
 *
 * Instructions are registered in the instruction registry.
 */

/* -----------------------------------------------------------
 * Instruction opcode enumeration
 * -----------------------------------------------------------
 */
typedef enum {
    VM_NOP = 0,     /* no operation */
    VM_WRITE,       /* write arg to tape */
    VM_MOVE_RIGHT,  /* head := head + 1 */
    VM_MOVE_LEFT,   /* head := head - 1 */
    VM_ADD,         /* tape[head] := tape[head] + arg */
    VM_SUB,         /* tape[head] := tape[head] - arg */
    VM_MAX_OPCODE   /* sentinel */
} VMOpcode;

/* -----------------------------------------------------------
 * Instruction handler signature
 * -----------------------------------------------------------
 *
 * All instructions follow this signature:
 *
 *   int handler(CoreTape *tape, int arg);
 *
 * Return:
 *   0 on success
 *   nonzero on failure
 */
typedef int (*VMInstructionHandler)(CoreTape *tape, int arg);

/* -----------------------------------------------------------
 * Instruction descriptor
 * -----------------------------------------------------------
 */
typedef struct {
    VMOpcode opcode;
    const char *name;
    VMInstructionHandler handler;
} VMInstruction;

/* -----------------------------------------------------------
 * Registry API
 * -----------------------------------------------------------
 */
int vm_register_instruction(VMOpcode opcode,
                            const char *name,
                            VMInstructionHandler handler);

const VMInstruction *vm_get_instruction(VMOpcode opcode);

#endif /* PHOENIX_VM_INSTRUCTION_H */
