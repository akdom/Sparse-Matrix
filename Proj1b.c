#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Entry Entry;
struct Entry {
  unsigned long row, col;
  double val;
};

void get_entry(Entry *entry, FILE *input_file);
void get_dimensions(long *dim, long *entries, FILE *input_file);
void print_matrix(Entry matrix[], long entries);
int row_sort(const void *_a, const void *_b);
int col_sort(const void *_a, const void *_b);
void right_mult(Entry matrix[], long entries, double vector[], double out[]);
void left_mult(Entry matrix[], long entries, double vector[], double out[]);


int main (int argc, char *argv[]) {
    long dim, entries;
    clock_t time = clock();
    
    char *filename = argv[1];
    if (filename == NULL) {
        printf("THE END OF THE WORLD IS UPON US... THE ONE TRUE FILE DOES NOT EXIST!!! (or hasn't been handed as the first argument...)");
        exit(-1);
    }
    
    FILE *input_file = fopen(filename, "r");
    get_dimensions(&dim, &entries, input_file);
    
    //Create our matrix
    Entry matrix[entries];
    double source[dim], dest[dim];
        
    int i;
    for(i=0; i < entries; i++) {
        get_entry(&matrix[i], input_file);	
    }
    printf("Time taken to load matrix: %lfs\n", (clock()-time)/(double)CLOCKS_PER_SEC);
    for(i=0; i < dim; i+=2) {
	source[i]=1.0/(dim/2.0);
    }
        
    left_mult(matrix, entries, source, dest);
    
    fclose(input_file);
    return (0);
}

void get_dimensions(long *dim, long *entries, FILE *input_file) {
    int count = fscanf(input_file, "%ld %ld", dim, entries);
    if (count == 2)
        return;
    else
        return;
}

void get_entry(Entry *entry, FILE *input_file) {
    fscanf(input_file, "%lf %lu %lu", &(entry->val), &(entry->col), &(entry->row));
}

int row_sort(const void *a, const void *b) {
    return (((Entry*)a)->row - ((Entry*)b)->row) ?
	(((Entry*)a)->row - ((Entry*)b)->row) :
	(((Entry*)a)->col - ((Entry*)b)->col);
}

int col_sort(const void *a, const void *b) {
    return (((Entry*)a)->col - ((Entry*)b)->col) ?
	(((Entry*)a)->col - ((Entry*)b)->col) :
	(((Entry*)a)->row - ((Entry*)b)->row);	
}

void print_matrix(Entry matrix[], long entries) {
    int i;
    for(i=0; i<entries; i++) {
	printf("Val: %lf; Col: %lu; Row: %lu;\n", matrix[i].val, matrix[i].col, matrix[i].row);
    }
}

void right_mult(Entry matrix[], long entries, double vector[], double out[]) {
    int i;
    Entry *e;
    for(i=0, e=matrix; i<entries; i++, e++) {
	out[e->row] += e->val * vector[e->col];
    }
}

void left_mult(Entry matrix[], long entries, double vector[], double out[]) {
    int i;
    Entry *e;
    for(i=0, e=matrix; i<entries; i++, e++) {
	out[e->col] += e->val * vector[e->row];
    }
}

void normalize(double vector[], long dim) {
    double total;
    int i;
    for(i=0; i<dim; i++) {
	total += vector[i];
    }
    for(i=0; i<dim; i++) {
	vector[i] /= total;
    }
}

void normalize2(double vector[], long dim) {
    double *cur, *last, total;
    last = &vector[dim];
    for(cur=vector; cur<=last; cur++) {
	total += *cur;
    }
    for(cur=vector; cur<=last; cur++) {
	*cur /= total;
    }
}
