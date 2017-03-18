#ifndef RB_TREE_H
#define RB_TREE_H
#include <stdbool.h>

typedef struct RBNode {
	enum { RB_RED, RB_BLACK } colour;
	void *         key;
	void *         value;
	struct RBNode *left;
	struct RBNode *right;
} RBNode;

// <0, 0, or >0 if a is  (<, =, and >) b
typedef int (*rb_cmp)(const void *a, const void *b);
typedef void (*rb_delete_key)(void *key);

// Wrapper so that we can keep track of the size of the tree.
// Also saves passing around th cmp function everywhere.
typedef struct RBTree {
	RBNode *      root;
	int           size;
	rb_cmp        cmp;
	rb_delete_key delete_key;
} RBTree;

// malloc and init a tree
RBTree *rb_alloc(rb_cmp cmp, rb_delete_key delete_key);

// O(1)
bool rb_empty(RBTree *tree);
// O(1)
int rb_size(RBTree *tree);

// O(log n)
bool rb_insert(RBTree *tree, void *key);
// O(log n)
bool rb_delete(RBTree *tree, void *key);

// O(log n)
void *rb_find(RBTree *tree, void *key);
// O(log n) allows a value to replaced inplace.
RBNode *rb_extract(RBTree *tree, void *key);

// Function that work on nodes

// malloc a new node
RBNode *rb_node_alloc(void *key, void *value);

// tree transversal
typedef void (*rb_vistor)(RBNode *n, void *extra);
void rb_node_inorder(RBNode *n, rb_vistor visit, void *extra);

// comparison functions
int rb_cmp_int(const void *a, const void *b);

#endif /* RB_TREE_H */
