#include "rb_node.h"
#include "rb_print.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

RBTree *rb_alloc(rb_cmp cmp, rb_delete_key delete_key) {
	RBTree *root = malloc(sizeof(RBTree));
	root->root   = NULL;
	root->size   = 0;

	root->cmp        = cmp;
	root->delete_key = delete_key;

	return root;
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