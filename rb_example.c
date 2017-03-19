#include "rb_node.h"
#include "rb_print.h"
#include <stdio.h>
#include <stdlib.h>

// We are allocating on the stack
void nop(void *ptr) {}

int main(int argc, char const *argv[]) {
	RBTree *tree  = rb_alloc(rb_cmp_int, nop, free);
	int     arr[] = {100, 50, 150, 25, 75, 125, 175, 110, 22, 11, 33};

	for (int i = 0; i < sizeof(arr) / sizeof(int); i++) {
		int *value = malloc(sizeof(int));
		*value     = arr[i] * 10;
		printf("   Inserting %03d:%03d \n", arr[i], *value);
		rb_insert(tree, &arr[i], value);
		rb_print_diagram_int(tree->root);
		printf("   rb_validate %d\n", rb_validate(tree));
		puts("");
	}

	rb_free(tree);

	return 0;
}