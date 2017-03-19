#include "rb_node.h"
#include "rb_print.h"
#include <stdio.h>
#include <stdlib.h>

// We are allocating the keys on the stack
static void nop(void *ptr) {}

// Prints a node
static void printer(RBNode *n, void *unused) {
	int *k = n->key;
	int *v = n->value;
	printf("%03d -> %03d\n", *k, *v);
}

int main(int argc, char const *argv[]) {
	RBTree *tree  = rb_alloc(rb_cmp_int, nop, free);
	int     arr[] = {100, 50, 150, 25, 75, 125, 175, 110, 22, 11, 33};

	// Inserting an unordered array
	for (int i = 0; i < sizeof(arr) / sizeof(int); i++) {
		int *value = malloc(sizeof(int));
		*value     = arr[i] * 10;
		printf("   Inserting %03d:%03d \n", arr[i], *value);
		rb_insert(tree, &arr[i], value);
		// rb_print_diagram_int(tree->root);
		printf("   rb_validate %d\n", rb_validate(tree));
		puts("");
	}

	puts("Diagram of the tree (with colour)");
	rb_print_diagram_int(tree->root);
	puts("");

	// Finding every 4th element
	for (int i = 0; i < sizeof(arr) / sizeof(int); i += 4) {
		printf("   Searching for %03d \n", arr[i]);
		int *res = rb_find(tree, &arr[i]);
		if (res != NULL) {
			printf("   Found   for key:%03d  %03d\n", arr[i], *res);
		} else {
			printf("   Missing for key:%03d \n", arr[i]);
		}

		printf("   rb_validate %d\n", rb_validate(tree));
		puts("");
	}

	int *min_elem = rb_min(tree);
	printf("min key %d\n", *min_elem);

	puts("");
	// Deleting every 4th element
	for (int i = 0; i < sizeof(arr) / sizeof(int); i += 4) {
		printf("   Deleting  %03d \n", arr[i]);
		bool res = rb_delete(tree, &arr[i]);
		printf("   >Result  %d\n", res);
		// rb_print_diagram_int(tree->root);
		printf("   rb_validate %d\n", rb_validate(tree));
		puts("");
	}

	puts("Diagram of the tree (with colour)");
	rb_print_diagram_int(tree->root);
	puts("");
	puts("Printing elements in sorted order");
	rb_node_inorder(tree->root, printer, NULL);

	rb_free(tree);

	return 0;
}