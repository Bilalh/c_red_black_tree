#include "rb_node.h"
#include "rb_print.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// To hide debug output
#define RB_TESTING false
#define dprintf(fmt, ...)                                                                \
	do {                                                                                 \
		if (RB_TESTING) {                                                                \
			fprintf(stderr, fmt, ##__VA_ARGS__);                                         \
		}                                                                                \
	} while (0)

// static functions
// needed by other methods

// Right -> left
static RBNode *rotate_left(RBTree *tree, RBNode *n);
// Left -> right
static RBNode *rotate_right(RBTree *tree, RBNode *n);
// Invert the colours of a nodes and its children
static void flip(RBTree *tree, RBNode *n);

// Red -> Black and vice versa
static RBNode *rebalance(RBTree *tree, RBNode *n);
// Min node rooted at n
static RBNode *rb_min_node(RBNode *n);

// n is red
// n->left && n->left->left are black
// converts either n->left or it children to red
static RBNode *left_to_red(RBTree *tree, RBNode *n);

// n is red
// n->right && n->right->left are black
// converts either n->right or it children to red
static RBNode *right_to_red(RBTree *tree, RBNode *n);

// Remove and return the min node rooted at n
static RBNode *remove_min(RBTree *tree, RBNode *n);

// init/free

RBTree *rb_alloc(rb_cmp cmp, rb_freer free_key, rb_freer free_value) {
	RBTree *root = malloc(sizeof(RBTree));
	root->root   = NULL;
	root->size   = 0;

	root->cmp        = cmp;
	root->free_key   = free_key;
	root->free_value = free_value;
	return root;
}

static void rb_free_sub(RBNode *n, void *tree_void);
void rb_free(RBTree *tree) {
	if (tree == NULL) {
		return;
	}
	rb_node_postorder(tree->root, rb_free_sub, tree);
	free(tree);
}

static void rb_free_sub(RBNode *n, void *tree_void) {
	RBTree *tree = tree_void;
	dprintf("Freeing %d\n", *(int *)n->key);
	tree->free_key(n->key);
	tree->free_value(n->value);
	free(n);
}

// size

bool rb_empty(RBTree *root) { return root->size == 0; }

int rb_size(RBTree *root) {
	if (root == NULL) {
		return 0;
	}
	return root->size;
}

// find

RBNode *rb_extract(RBTree *tree, void *key) {
	if (tree == NULL || key == NULL || rb_empty(tree)) {
		return NULL;
	}

	RBNode *cur = tree->root;
	while (cur != NULL) {
		int cmp = tree->cmp(key, cur->key);

		if (cmp < 0) {
			cur = cur->left;
		} else if (cmp > 0) {
			cur = cur->right;
		} else {
			return cur;
		}
	}

	return NULL;
}

void *rb_find(RBTree *tree, void *key) {
	RBNode *ans = rb_extract(tree, key);

	if (ans != NULL) {
		return ans->value;
	} else {
		return NULL;
	}
}

void *rb_min(RBTree *tree) {
	if (tree == NULL) {
		return NULL;
	}
	RBNode *res = rb_min_node(tree->root);
	if (res) {
		return res->key;
	} else {
		return NULL;
	}
}

static RBNode *rb_min_node(RBNode *n) {
	while (n) {
		if (n->left == NULL) {
			return n;
		}
		n = n->left;
	}
	return NULL;
}

// insert
static RBNode *rb_insert_sub(RBTree *tree, RBNode *n, void *key, void *value);
bool rb_insert(RBTree *tree, void *key, void *value) {
	assert(tree);
	assert(key);

	RBNode *res = rb_insert_sub(tree, tree->root, key, value);
	if (res != NULL) {
		tree->root         = res;
		tree->root->colour = RB_BLACK;
		tree->size++;
		return true;
	} else {
		return false;
	}
}

static RBNode *rb_insert_sub(RBTree *tree, RBNode *n, void *key, void *value) {
	if (n == NULL) {
		return rb_node_alloc(key, value);
	}

	int cmp = tree->cmp(key, n->key);
	if (cmp < 0) {
		n->left = rb_insert_sub(tree, n->left, key, value);
	} else if (cmp > 0) {
		n->right = rb_insert_sub(tree, n->right, key, value);
	} else {
		n->value = value;
	}

	if (is_red(n->right) && !is_red(n->left)) {
		n = rotate_left(tree, n);
	}
	if (is_red(n->left) && is_red(n->left->left)) {
		n = rotate_right(tree, n);
	}
	if (is_red(n->left) && is_red(n->right)) {
		flip(tree, n);
	}
	return n;
}

static RBNode *rotate_left(RBTree *tree, RBNode *n) {
	assert(tree);
	assert(n);
	RBNode *r = n->right;
	n->right  = r->left;
	r->left   = n;
	r->colour = r->left->colour;

	r->left->colour = RB_RED;
	return r;
}

static RBNode *rotate_right(RBTree *tree, RBNode *n) {
	assert(tree);
	assert(n);
	RBNode *r = n->left;
	n->left   = r->right;
	r->right  = n;
	r->colour = r->right->colour;

	r->right->colour = RB_RED;
	return r;
}

static void flip(RBTree *tree, RBNode *n) {
	assert(tree);
	assert(n);
	n->colour        = invert_colour(n->colour);
	n->left->colour  = invert_colour(n->left->colour);
	n->right->colour = invert_colour(n->right->colour);
}

RBColour invert_colour(RBColour c) {
	if (c == RB_BLACK) {
		return RB_RED;
	} else {
		return RB_BLACK;
	}
}

bool is_red(RBNode *n) {
	if (n == NULL) {
		return false;
	} else {
		return n->colour == RB_RED;
	}
}

bool is_black(RBNode *n) {
	if (n == NULL) {
		return false;
	} else {
		return n->colour == RB_BLACK;
	}
}

// delete

static RBNode *rb_delete_sub(RBTree *tree, RBNode *n, void *key);
bool rb_delete(RBTree *tree, void *key) {
	assert(tree);
	assert(key);

	if (!rb_find(tree, key)) {
		return false;
	}

	if (!is_red(tree->root->left) && !is_red(tree->root->right)) {
		tree->root->colour = RB_RED;
	}

	tree->root = rb_delete_sub(tree, tree->root, key);
	if (!rb_empty(tree)) {
		tree->root->colour = RB_BLACK;
	}
	tree->size--;
	return true;
}

static RBNode *rb_delete_sub(RBTree *tree, RBNode *n, void *key) {
	assert(tree);
	assert(n);
	assert(key);

	if (tree->cmp(key, n->key) < 0) {
		if (!is_red(n->left) && !is_red(n->left->left)) {
			n = left_to_red(tree, n);
		}
		n->left = rb_delete_sub(tree, n->left, key);
	} else {
		if (is_red(n->left)) {
			n = rotate_right(tree, n);
		}
		if (tree->cmp(key, n->key) == 0 && n->right == NULL) {
			tree->free_key(n->key);
			tree->free_value(n->value);
			free(n);
			return NULL;
		}
		if (!is_red(n->right) && !is_red(n->right->left)) {
			n = right_to_red(tree, n);
		}
		if (tree->cmp(key, n->key) == 0) {
			RBNode *m = rb_min_node(n->right);
			tree->free_key(n->key);
			tree->free_value(n->value);
			n->key   = m->key;
			n->value = m->value;
			n->right = remove_min(tree, n->right);
			free(m);
		} else {
			n->right = rb_delete_sub(tree, n->right, key);
		}
	}

	return rebalance(tree, n);
}

static RBNode *left_to_red(RBTree *tree, RBNode *n) {
	assert(tree);
	assert(n);

	flip(tree, n);
	if (is_red(n->right->left)) {
		n->right = rotate_right(tree, n->right);
		n        = rotate_left(tree, n);
		flip(tree, n);
	}
	return n;
}

static RBNode *right_to_red(RBTree *tree, RBNode *n) {
	assert(tree);
	assert(n);

	flip(tree, n);
	if (is_red(n->left->left)) {
		n = rotate_right(tree, n);
		flip(tree, n);
	}
	return n;
}

RBNode *remove_min(RBTree *tree, RBNode *n) {
	assert(tree);
	assert(n);
	if (n->left == NULL) {
		return NULL;
	}

	if (!is_red(n->left) && !is_red(n->left->left)) {
		n = left_to_red(tree, n);
	}
	n->left = remove_min(tree, n->left);
	return rebalance(tree, n);
}

static RBNode *rebalance(RBTree *tree, RBNode *n) {
	assert(tree);
	assert(n);

	if (is_red(n->right)) {
		n = rotate_left(tree, n);
	}
	if (is_red(n->left) && is_red(n->left->left)) {
		n = rotate_right(tree, n);
	}
	if (is_red(n->left) && is_red(n->right)) {
		flip(tree, n);
	}
	return n;
}

// Node

RBNode *rb_node_alloc(void *key, void *value) {
	RBNode *n = malloc(sizeof(RBNode));
	n->key    = key;
	n->value  = value;
	n->left   = NULL;
	n->right  = NULL;
	n->colour = RB_RED;
	return n;
}

void rb_node_inorder(RBNode *n, rb_vistor visit, void *extra) {
	if (n == NULL) {
		return;
	}

	rb_node_inorder(n->left, visit, extra);
	visit(n, extra);
	rb_node_inorder(n->right, visit, extra);
}

void rb_node_postorder(RBNode *n, rb_vistor visit, void *extra) {
	if (n == NULL) {
		return;
	}

	rb_node_postorder(n->left, visit, extra);
	rb_node_postorder(n->right, visit, extra);
	visit(n, extra);
}

// Validation

// Checks that we kept track of the size correctly
static bool vaildate_size(RBTree *tree);

bool rb_validate(RBTree *tree) {
	bool res = true;
	if (tree == NULL) {
		return res;
	}

	if ((tree->size == 0 && tree->root != NULL) ||
		(tree->root != NULL && tree->size == 0)) {
		fprintf(stderr, "rb_validate: size inconsistent\n");
		res = false;
	}

	res &= vaildate_size(tree);

	if (!rb_is_bst(tree)) {
		fprintf(stderr, "rb_validate: not a BST\n");
		res = false;
	}

	if (!rb_is_balanced(tree)) {
		fprintf(stderr, "rb_validate: not balanced\n");
		res = false;
	}

	return res;
}

static void update_count(RBNode *n, void *extra) {
	int *count = (int *)extra;
	(*count)++;
}

static bool vaildate_size(RBTree *tree) {
	int count = 0;
	rb_node_inorder(tree->root, update_count, &count);
	if (count != tree->size) {
		fprintf(stderr, "rb_validate: expected %03d is %03d\n", count, tree->size);
		return false;
	}
	return true;
}

static bool rb_is_bst_sub(RBTree *tree, RBNode *n, void *min, void *max);
bool rb_is_bst(RBTree *tree) { return rb_is_bst_sub(tree, tree->root, NULL, NULL); }

static bool rb_is_bst_sub(RBTree *tree, RBNode *n, void *min, void *max) {
	if (n == NULL) {
		return true;
	}
	if (min != NULL && tree->cmp(n->key, min) <= 0) {
		return false;
	}
	if (max != NULL && tree->cmp(n->key, max) >= 0) {
		return false;
	}
	return rb_is_bst_sub(tree, n->left, min, n->key) &&
		   rb_is_bst_sub(tree, n->right, n->key, max);
}

static bool rb_is_balanced_sub(RBTree *tree, RBNode *node, int black);
bool rb_is_balanced(RBTree *tree) {
	int black = 0;

	//  # black links to min value
	RBNode *n = tree->root;
	while (n != NULL) {
		if (!is_red(n)) {
			black++;
		}
		n = n->left;
	}
	dprintf("rb_validate: %d black\n", black);
	return rb_is_balanced_sub(tree, tree->root, black);
}

static bool rb_is_balanced_sub(RBTree *tree, RBNode *n, int black) {
	if (n == NULL) {
		dprintf("rb_validate_sub: n %15p        black %03d  \n", n, black);
		return black == 0;
	}
	int k = *(int *)n->key;
	dprintf("rb_validate_sub: n %15p {%03d}  black %03d  \n", n, k, black);

	if (!is_red(n)) {
		black--;
	}
	return rb_is_balanced_sub(tree, n->left, black) &&
		   rb_is_balanced_sub(tree, n->right, black);
}

// Comparison

int rb_cmp_int(const void *a, const void *b) {
	const int arg1 = *(const int *)a;
	const int arg2 = *(const int *)b;

	if (arg1 < arg2) {
		return -1;
	} else if (arg1 > arg2) {
		return 1;
	} else {
		return 0;
	}
}