#ifndef ABR_PIPELINE_H
#define ABR_PIPELINE_H

#include "bigint_list.h"

/*
 * ABR pipeline interface
 *
 * All pipeline stages operate on BigIntList windows.
 */

int pipeline_init(const BigIntList *windows_in, BigIntList *windows_out);
int pipeline_linear(const BigIntList *windows_in, BigIntList *windows_out);
int pipeline_reverse(const BigIntList *windows_in, BigIntList *windows_out);
int pipeline_branching(const BigIntList *windows_in, BigIntList *windows_out);

#endif /* ABR_PIPELINE_H */
