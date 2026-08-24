#ifndef ABR_BIGINT_LIST_H
#define ABR_BIGINT_LIST_H

#include <stddef.h>
#include "bigint.h"

/*
 * BigIntList (Phoenix Edition)
 *
 * A dynamic array of BigInt objects.
 *
 * Used for:
 *   - window extraction (each window is a BigInt)
 *   - pipeline stages (each stage transforms lists)
 *   - reconstruction (consumes windows)
 *
 * Math / Engineering view:
 *   A BigIntList is a finite sequence:
 *
 *       L = [x_0, x_1, ..., x_{n-1}]
 *
 *   where each x_i is a BigInt.
 *
 *   The list supports:
 *     - initialization
 *     - append
 *     - clear
 *     - free
 */
typedef struct {
    BigInt *items;   /* dynamic array of BigInt */
    size_t  count;   /* number of items */
    size_t  capacity;/* allocated capacity */
} BigIntList;

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 *
 * Initializes an empty list.
 *
 * Math view:
 *   L := [].
 */
void bigint_list_init(BigIntList *list);

/* -----------------------------------------------------------
 * Append
 * -----------------------------------------------------------
 *
 * Appends a BigInt to the list.
 *
 * Math view:
 *   L := L ⧺ [x].
 *
 * Memory:
 *   - The list owns the appended BigInt (deep copy).
 */
int bigint_list_append(BigIntList *list, const BigInt *src);

/* -----------------------------------------------------------
 * Clear
 * -----------------------------------------------------------
 *
 * Clears all BigInts but keeps capacity.
 *
 * Math view:
 *   L := [].
 */
void bigint_list_clear(BigIntList *list);

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 *
 * Frees all BigInts and the array.
 *
 * Math view:
 *   Releases the sequence L entirely.
 */
void bigint_list_free(BigIntList *list);

#endif /* ABR_BIGINT_LIST_H */
