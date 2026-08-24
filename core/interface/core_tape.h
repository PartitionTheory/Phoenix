#ifndef PHOENIX_CORE_TAPE_H
#define PHOENIX_CORE_TAPE_H

#include <stddef.h>

/*
 * Phoenix Core Tape Interface
 *
 * This is the minimal, stable ABI that the VM and System
 * layers depend on. The underlying implementation lives
 * in core/interface/core_tape.c.
 */

typedef struct {
    struct {
        int *cells;
        size_t capacity;
        size_t head;
    } impl;
} CoreTape;

/* Initialize tape with given capacity, all cells zeroed, head at 0 */
int core_tape_init(CoreTape *t, size_t capacity);

/* Read value at current head into *out */
int core_tape_read(CoreTape *t, int *out);

/* Write value at current head */
int core_tape_write(CoreTape *t, int value);

/* Move head one cell left (fails if already at 0) */
int core_tape_move_left(CoreTape *t);

/* Move head one cell right (fails if beyond capacity - 1) */
int core_tape_move_right(CoreTape *t);

/* Reset tape: zero all cells, head → 0 */
void core_tape_reset(CoreTape *t);

/* Free tape resources */
void core_tape_free(CoreTape *t);

#endif /* PHOENIX_CORE_TAPE_H */
