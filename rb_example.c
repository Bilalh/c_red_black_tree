#include "rb_node.h"
#include "rb_print.h"
#include <stdio.h>
#include <stdlib.h>

static RBNode *process(int *arr, int low, int high);

RBNode *array_to_bst(int *arr, int length) {
	if (!arr || length == 0) {
		return NULL;
	}

	qsort(arr, length, sizeof(int), rb_cmp_int);
	// for(size_t i= 0; i < length; ++i ){
	// 	printf("%zu :%d\n", i, arr[i]);
	// }

	int low  = 0;
	int high = length - 1;
	int mid  = (low + high) / 2;

	RBNode *root = rb_node_alloc(&arr[mid], &arr[mid]);
	// printf("%d, %d, %d\n", low, high,mid);
	root->left  = process(arr, low, mid - 1);
	root->right = process(arr, mid + 1, high);

	return root;
}

static RBNode *process(int *arr, int low, int high) {
	int mid = (low + high) / 2;
	// printf("%d, %d, %d\n", low, high, mid);
	if (!arr || low > high) {
		return NULL;
	}
	RBNode *n = rb_node_alloc(&arr[mid], &arr[mid]);
	if (low != high) {
		n->left  = process(arr, low, mid - 1);
		n->right = process(arr, mid + 1, high);
	}
	return n;
}

void nop(void *ptr) {}

int main(int argc, char const *argv[]) {
	int     arr[] = {100, 50, 150, 25, 75, 125, 175, 110};
	RBNode *root  = array_to_bst(arr, sizeof(arr) / sizeof(int));
	rb_print_diagram_int(root);
	puts("");

	RBTree *tree = rb_alloc(rb_cmp_int, nop, nop);
	tree->root   = root;
	tree->size   = sizeof(arr) / sizeof(int);

	int  key = 150;
	int *val = rb_find(tree, &key);
	if (val != NULL) {
		printf("Value %d found for key %d\n", *val, key);
	} else {
		printf("No value found for key %d\n", key);
	}
	printf("");
	rb_free(tree);

	return 0;
}
