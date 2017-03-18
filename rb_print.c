#include "rb_print.h"
#include "rb_node.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Colour used in printing
#ifdef NO_COLOURS
#define PRINT_RED ""
#define PRINT_BLACK ""
#define PRINT_RESET ""
#else
#define PRINT_RED "\x1B[31m"
#define PRINT_BLACK "\033[1m"
#define PRINT_RESET "\x1B[0m"
#endif

void rb_print_diagram(RBNode *n, RBPrint *rb, int start) {
	if (n == NULL || rb == NULL) {
		return;
	}

	// Spacing between nodes
	int GAP = (rb->data_max_len + rb->padding);

	for (int i = 0; i < start / GAP; i++) {
		printf("|%*s", GAP, "");
	}

	char *val_str = rb->data_str(n->key);
	printf("|%s%s%s", n->colour == RB_BLACK ? PRINT_BLACK : PRINT_RED, val_str,
		   PRINT_RESET);

	if (n->left || n->right) {
		int upper = rb->padding + 1 + (rb->data_max_len - strlen(val_str));
		for (int i = 0; i < upper; i++) {
			putchar('-');
		}
	}

	rb->data_str_free(val_str);

	putchar('\n');
	rb_print_diagram(n->left, rb, start + GAP);
	rb_print_diagram(n->right, rb, start + GAP);
}

/// For printing int trees

static int number_of_digits(void *num_ptr) {
	int num = *(int *)num_ptr;

	int add = 1;
	if (num < 0) {
		num = abs(num);
		add++;
	} else if (num == 0) {
		return 1;
	}
	int digits = floor(log10(num)) + add;
	return digits;
}

// converts a int to a malloc'd string
static char *int_to_str(void *num_ptr) {
	int   digits = number_of_digits(num_ptr);
	int   num    = *(int *)num_ptr;
	char *buff   = malloc(digits + 1);
	snprintf(buff, digits + 1, "%d", num);
	return buff;
}

static void int_to_str_free(char *num_ptr) { free(num_ptr); }

static void update_max(RBNode *n, void *extra) {
	int *cur_max = (int *)extra;
	int  digits  = number_of_digits(n->key);
	if (digits > *cur_max) {
		*cur_max = digits;
	}
}

void rb_print_diagram_int(RBNode *n) {
	RBPrint *rb = malloc(sizeof(RBPrint));

	int current_max_digits = 0;
	rb_node_inorder(n, update_max, &current_max_digits);

	rb->padding       = 4;
	rb->data_max_len  = current_max_digits;
	rb->data_str      = int_to_str;
	rb->data_str_free = int_to_str_free;
	rb_print_diagram(n, rb, 0);
	free(rb);
}