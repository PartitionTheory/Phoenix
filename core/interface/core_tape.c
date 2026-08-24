#include "core_tape.h"
#include <stdlib.h>
#include <string.h>

/*
 * Phoenix Core Tape Implementation
 *
 * This is a standalone tape engine used by the VM and System layers.
 * It does NOT depend on the old ABR tape subsystem.
 */

int core_tape_init(CoreTape *t, size_t capacity)
{
    t->impl.cells = calloc(capacity, sizeof(int));
    if (!t->impl.cells)
        return -1;

    t->impl.capacity = capacity;
    t->impl.head = 0;
    return 0;
}

int core_tape_read(CoreTape *t, int *out)
{
    if (!t || !out)
        return -1;

    *out = t->impl.cells[t->impl.head];
    return 0;
}

int core_tape_write(CoreTape *t, int value)
{
    if (!t)
        return -1;

    t->impl.cells[t->impl.head] = value;
    return 0;
}

int core_tape_move_left(CoreTape *t)
{
    if (t->impl.head == 0)
        return -1;

    t->impl.head--;
    return 0;
}

int core_tape_move_right(CoreTape *t)
{
    if (t->impl.head + 1 >= t->impl.capacity)
        return -1;

    t->impl.head++;
    return 0;
}

void core_tape_reset(CoreTape *t)
{
    memset(t->impl.cells, 0, sizeof(int) * t->impl.capacity);
    t->impl.head = 0;
}

void core_tape_free(CoreTape *t)
{
    free(t->impl.cells);
    t->impl.cells = NULL;
    t->impl.capacity = 0;
    t->impl.head = 0;
}
