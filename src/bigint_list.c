#include <stdlib.h>
#include <string.h>
#include "bigint_list.h"

/*
 * BigIntList (Phoenix Edition)
 *
 * Implements:
 *   bigint_list_init()
 *   bigint_list_append()
 *   bigint_list_clear()
 *   bigint_list_free()
 *
 * Math / Engineering view:
 *   A BigIntList is a finite sequence:
 *
 *       L = [x_0, x_1, ..., x_{n-1}]
 *
 *   with dynamic capacity management.
 */

/* -----------------------------------------------------------
 * Initialization
 * -----------------------------------------------------------
 */
void bigint_list_init(BigIntList *list)
{
    list->items = NULL;
    list->count = 0;
    list->capacity = 0;
}

/* -----------------------------------------------------------
 * Append
 * -----------------------------------------------------------
 *
 * Performs:
 *   L := L ⧺ [x]
 *
 * Deep copy semantics:
 *   - Allocate new BigInt
 *   - Copy src into new element
 */
int bigint_list_append(BigIntList *list, const BigInt *src)
{
    /* Grow capacity if needed */
    if (list->count == list->capacity) {
        size_t new_cap = (list->capacity == 0) ? 4 : list->capacity * 2;
        BigInt *new_items = realloc(list->items, new_cap * sizeof(BigInt));
        if (!new_items)
            return -1;

        list->items = new_items;
        list->capacity = new_cap;
    }

    /* Deep copy */
    BigInt *dst = &list->items[list->count];
    bigint_init(dst);

    if (bigint_copy(dst, src) != 0)
        return -2;

    list->count++;
    return 0;
}

/* -----------------------------------------------------------
 * Clear
 * -----------------------------------------------------------
 *
 * Clears list but keeps capacity.
 */
void bigint_list_clear(BigIntList *list)
{
    for (size_t i = 0; i < list->count; i++)
        bigint_free(&list->items[i]);

    list->count = 0;
}

/* -----------------------------------------------------------
 * Free
 * -----------------------------------------------------------
 *
 * Frees all BigInts and the array.
 */
void bigint_list_free(BigIntList *list)
{
    bigint_list_clear(list);
    free(list->items);
    list->items = NULL;
    list->capacity = 0;
}
