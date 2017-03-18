#include "rb_node.h"
#include "rb_print.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

RBTree *rb_alloc(rb_cmp cmp, rb_freer free_key, rb_freer free_value) {
	RBTree *root = malloc(sizeof(RBTree));
	root->root   = NULL;
	root->size   = 0;

	root->cmp        = cmp;
	root->free_key   = free_key;
	root->free_value = free_value;

	return root;
}

static void rb_free_sub(RBNode *n, void *tree_void) {
	RBTree *tree = tree_void;
	tree->free_key(n->key);
	tree->free_value(n->value);
	free(n);
}

void rb_free(RBTree *tree) {
	if (tree == NULL) {
		return;
	}
	rb_node_postorder(tree->root, rb_free_sub, tree);
	free(tree);
}

bool rb_empty(RBTree *root) { return root->size == 0; }

int rb_size(RBTree *root) {
	if (root == NULL) {
		return 0;
	}
	return root->size;
}

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

// Node function

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

// Comparison functions

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