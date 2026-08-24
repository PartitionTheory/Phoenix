#ifndef BRANCH_MERGE_SPLIT_H
#define BRANCH_MERGE_SPLIT_H

#include "abr_plugin.h"

/*
A: Mathematical description
   Given windows W0, W1, W2, W3:
     - Merge W0 and W1: M = concat(W0, W1)
     - Split W2 into two 3-bit windows: S1 = W2[0..2], S2 = W2[3..5]
     - Drop W3
   Output WindowSet = { M, S1, S2 }

B: Engineering description
   This is a branching transform:
   - Allocates a new WindowSet
   - Allocates new bit buffers
   - Sets out_count = 3
*/

Plugin *make_branch_merge_split_plugin(void);

#endif

