#ifndef RB_ARRAY_H
#define RB_ARRAY_H

#include "rb_node.h"

// O(N)
// Constructs a red black tree from a sorted array.
void rb_from_sorted_array(RBTree *tree, int *arr, int length);

#endif /* RB_ARRAY_H */
