#include "rb_array.h"
#include "rb_node.h"
#include "rb_print.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static RBNode *process(int *arr, int low, int high, int left);
static RBNode *from_sorted_array(int *arr, int length);

void rb_from_sorted_array(RBTree *tree, int *arr, int length) {
	tree->root = from_sorted_array(arr, length);
	tree->size = length;
}

static RBNode *from_sorted_array(int *arr, int length) {
	// The idea is to split the array recursively to create the trees.

	if (!arr || length == 0) {
		return NULL;
	}

	int low  = 0;
	int high = length - 1;
	int mid  = (low + high) / 2;

	RBNode *root = rb_node_alloc(&arr[mid], &arr[mid]);

	// Since it will be balanced we can calculate the depth of the lowest level.
	int depth   = log2(length);
	root->left  = process(arr, low, mid - 1, depth);
	root->right = process(arr, mid + 1, high, depth);

	root->colour = RB_BLACK;

	return root;
}

static RBNode *process(int *arr, int low, int high, int left) {
	int mid = (low + high) / 2;

	if (!arr || low > high) {
		return NULL;
	}

	RBNode *n = rb_node_alloc(&arr[mid], &arr[mid]);
	n->colour = RB_BLACK;

	if (low != high) {
		n->left  = process(arr, low, mid - 1, left - 1);
		n->right = process(arr, mid + 1, high, left - 1);
	}
	if (left == 1) {
		n->colour = RB_RED;
	}

	return n;
}

// We are allocating the keys on the stack
static void nop(void *ptr) {}

int main(int argc, char const *argv[]) {
	int arr[] = {100, 50, 150, 25, 75, 125, 175, 110};
	int size  = sizeof(arr) / sizeof(int);
	qsort(arr, size, sizeof(int), rb_cmp_int);

	for (int i = 0; i < size; i++) {
		RBTree *tree = rb_alloc(rb_cmp_int, nop, nop);
		rb_from_sorted_array(tree, arr + i, size - i);
		rb_print_diagram_int(tree->root);

		printf("rb_validate %d\n", rb_validate(tree));
		puts(" ");
		rb_free(tree);
	}

	return 0;
}
