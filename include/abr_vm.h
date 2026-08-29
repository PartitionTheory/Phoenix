#ifndef ABR_VM_H
#define ABR_VM_H

/* Phoenix Structural Machine — VM Contract
 *
 * This header defines the structural interface for the Phoenix
 * execution chamber (VM). It contains no logic, only the types and
 * declarations needed to describe execution.
 */

#include "abr_core.h"
#include "abr_interface.h"

/* VM instruction opcode type */
typedef int phoenix_opcode;

/* VM instruction */
typedef struct {
    phoenix_opcode opcode;
    phoenix_value  operand;
} phoenix_instruction;

/* VM execution context */
typedef struct {
    phoenix_id     context_id;
    phoenix_value  accumulator;
    phoenix_bool   halted;
} phoenix_vm_context;

/* VM entry points (declarations only) */
phoenix_bool phoenix_vm_step(phoenix_vm_context *ctx,
                             const phoenix_instruction *instr);

phoenix_bool phoenix_vm_run(phoenix_vm_context *ctx,
                            const phoenix_instruction *program,
                            phoenix_int64 length);

#endif /* ABR_VM_H */

