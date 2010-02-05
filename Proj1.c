#include <stdio.h>
#include <stdlib.h>

struct Node {
	unsigned long col;
	double val;
	struct Node *next;
};

int main () {
	struct Node rows[4];
	rows[0].next = (struct Node *) malloc(sizeof(struct Node));
	rows[0].next->val = 2.3;
	rows[0].next->col = 3;
	printf("%d, %lf", rows[0].next->col, rows[0].next->val);
	return (0);
}


