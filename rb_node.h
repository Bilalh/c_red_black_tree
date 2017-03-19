#ifndef RB_TREE_H
#define RB_TREE_H
#include <stdbool.h>

typedef enum RBColour { RB_RED, RB_BLACK } RBColour;
typedef struct RBNode {
	RBColour       colour;
	void *         key;
	void *         value;
	struct RBNode *left;
	struct RBNode *right;
} RBNode;

// <0, 0, or >0 if a is  (<, =, and >) b
typedef int (*rb_cmp)(const void *a, const void *b);
typedef void (*rb_freer)(void *ptr);

// Wrapper so that we can keep track of the size of the tree.
// Also saves passing around cmp function everywhere.
typedef struct RBTree {
	RBNode * root;
	int      size;
	rb_cmp   cmp;
	rb_freer free_key;
	rb_freer free_value;
} RBTree;

// malloc and init a tree
RBTree *rb_alloc(rb_cmp cmp, rb_freer free_key, rb_freer free_value);
void rb_free(RBTree *tree);

// Size

// O(1)
bool rb_empty(RBTree *tree);
// O(1)
int rb_size(RBTree *tree);

// O(log n)
// return false of failure
bool rb_insert(RBTree *tree, void *key, void *value);
// O(log n)
void rb_delete(RBTree *tree, void *key);

// Find

// O(log n)
void *rb_find(RBTree *tree, void *key);
// O(log n) allows a value to replaced inplace.
RBNode *rb_extract(RBTree *tree, void *key);

// malloc a new node
RBNode *rb_node_alloc(void *key, void *value);

// Tree transversal
typedef void (*rb_vistor)(RBNode *n, void *extra);
void rb_node_inorder(RBNode *n, rb_vistor visit, void *extra);
void rb_node_postorder(RBNode *n, rb_vistor visit, void *extra);

// Validation
bool rb_validate(RBTree *tree);
bool rb_is_bst(RBTree *tree);
bool rb_is_balanced(RBTree *tree);

// Misc
int rb_cmp_int(const void *a, const void *b);
RBColour invert_colour(RBColour c);

#endif /* RB_TREE_H */
