#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;
struct Node {
	unsigned long col;
	double val;
	struct Node *next;
};

void get_dimensions(int *dim, int *entries, FILE *input_file);
int imprint_node(Node *new, FILE *input_file);
void insert_node(Node *matrix[], Node *new, int row);
void print_matrix(Node *matrix[]);

int main (int argc, char *argv[]) {
    int dim, entries;
    
    char *filename = argv[1];
    if (filename == NULL) {
        printf("THE END OF THE WORLD IS UPON US... THE ONE TRUE FILE DOES NOT EXIST!!! (or hasn't been handed as the first argument...)");
        exit(-1);
    }
    
    FILE *input_file = fopen(filename, "r");
    get_dimensions(&dim, &entries, input_file);
    
    //Create our matrix
    Node *matrix[dim];
    
    Node *new;
    int i;
    for(i=entries; i > 0; i--) {
        new = (Node *) malloc(sizeof(Node));
        insert_node(matrix, new, imprint_node(new, input_file));
    }
    
   /*
    Node rows[4];
    rows[0].next = (Node *) malloc(sizeof(Node));
    rows[0].next->val = 2.3;
    rows[0].next->col = 3;
    printf("%d, %lf", rows[0].next->col, rows[0].next->val);
    */
    
    fclose(input_file);
    return (0);
}

void get_dimensions(int *dim, int *entries, FILE *input_file) {
    int count = fscanf(input_file, "%d %d", dim, entries);
    if (count == 2)
        return;
    else
        return;
}

int imprint_node(Node *new, FILE *input_file) {
    int row;
    fscanf(input_file, "%lf %ld %d", &(new->val), &(new->col), &row);
    return row;
}

void insert_node(Node *matrix[], Node *new,int row) {
    if (matrix[row] == NULL) {
        matrix[row] = new;
    } else {
        Node *current = matrix[row];
        while (current->col < new->col) { current = current->next; }
        current->next = new;
    }
    return;
}

/*
void print_matrix(Node *matrix[], dim) {
    for() {
    }
}
*/
