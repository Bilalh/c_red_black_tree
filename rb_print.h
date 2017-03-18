#ifndef RB_PRINT_H
#define RB_PRINT_H
#include "rb_node.h"

// generic Functions to display tree
// with convenience functions for int trees
// Works on nodes so that a tree can be printed at any point.

// The nodes are actually in colour in a terminal
// |100-----
// |       |50------
// |       |       |25
// |       |       |75
// |       |125-----
// |       |       |110
// |       |       |150-----
// |       |       |       |175
void rb_print_diagram_int(RBNode *n);

// For generic printing:
// data_max_len  is the maximum length of any value when converted to a string.
// data_str      converts the value to string.
// data_str_free should free the return of data_str.
// padding       spacing between values
// start         should be 0

typedef char *(*data_str)(void *);
typedef void (*data_str_free)(char *);

typedef struct RBPrint {
	int			  padding;
	int			  data_max_len;
	data_str	  data_str;
	data_str_free data_str_free;
} RBPrint;

void rb_print_diagram(RBNode *n, RBPrint *rb, int start);
#endif /* RB_PRINT_H */
