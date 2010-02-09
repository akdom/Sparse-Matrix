#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DELTA_T(time) ( (clock()-time)/(double)CLOCKS_PER_SEC )
#define EPSILON 1e-6
#define ITERMAX 100

typedef struct Entry Entry;
struct Entry {
  unsigned long row, col;
  double val;
};

void get_entry(Entry *entry, FILE *input_file);
void get_dimensions(long *dim, long *entries, FILE *input_file);
void print_matrix(Entry matrix[], long entries);
void print_vect(double vect[], long dim);
int row_sort(const void *_a, const void *_b);
int col_sort(const void *_a, const void *_b);
void right_mult(Entry matrix[], long entries, double vector[], double out[]);
void left_mult(Entry matrix[], long entries, double vector[], double out[]);
void normalize(double vector[], long dim);
double vect_diff(double a[], double b[], long dim);
void calc_eigenvector(Entry matrix[], long entries, long dim, void (*mult)(Entry matrix[], long entries, double vector[], double out[]));

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
        
    int i;
    for(i=0; i < entries; i++) {
        get_entry(&matrix[i], input_file);	
    }
    printf("Time to load matrix: %lf\n", DELTA_T(time));

    fclose(input_file);

    time=clock();
    calc_eigenvector(matrix, entries, dim, &left_mult);
    printf("Time to compute left eigenvector: %lf\n", DELTA_T(time));

    time=clock();
    calc_eigenvector(matrix, entries, dim, &right_mult);
    printf("Time to compute right eigenvector: %lf\n", DELTA_T(time)); 

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

void print_vect(double vect[], long dim) {
    long i;
    for(i=0; i<dim; i++) {
	printf("i: %ld; val: %lg\n", i, vect[i]);
    }
}

void right_mult(Entry matrix[], long entries, double vector[], double out[]) {
    Entry *e;
    for(e=&matrix[entries-1]; e>=matrix; e--) {
	out[e->row-1] += e->val * vector[e->col-1];
    }
}

void left_mult(Entry matrix[], long entries, double vector[], double out[]) {
    Entry *e;
    for(e=&matrix[entries-1]; e>=matrix; e--) {
	out[(e->col)-1] += e->val * vector[(e->row)-1];
    }
}

void normalize(double vector[], long dim) {
    double *cur, *last, total=0;
    last = &vector[dim-1];
    for(cur=vector; cur<=last; cur++) {
	total += *cur;
	//printf("*cur: %lf\n", *cur); 
    }
    for(cur=vector; cur<=last; cur++) {
	*cur /= total;
    }
}

double vect_diff(double a[], double b[], long dim) {
    double *cur_a, *cur_b, *last_a, total=0;
    last_a = &a[dim-1];
    for(cur_a=a, cur_b=b; cur_a<=last_a; cur_a++, cur_b++) {
      //      printf("*cur_a: %lg\n", *cur_a);
      total += ((*cur_a - *cur_b)>0 ? (*cur_a - *cur_b) : (*cur_b - *cur_a));
    }
    return total;
}

void calc_eigenvector(Entry matrix[], long entries, long dim, void (*mult)(Entry matrix[], long entries, double vector[], double out[])) {
    double vector1[dim], vector2[dim];
    double *temp, *source, *dest, *d, diff;

    int i;
    for(i=0; i < dim; i++) {
      vector1[i]= 1.0 * ((i+1) % 2);
    }
    // start them in the "wrong" order, so that the first swap corrects it
    dest = vector1;
    source = vector2;
    //print_vect(dest, dim);

    normalize(dest, dim);

    for(i=0; i<ITERMAX; i++) {
	//swap the vectors
	temp = source;
	source = dest;
	dest = temp;
	for(d = &dest[dim-1]; d!=dest; d--) *d = 0.0; //clear dest
	

	mult(matrix, entries, source, dest);

	normalize(dest, dim);
	
	diff = vect_diff(dest, source, dim);
	printf("i: %d; |z-x|: %lg\n", i, diff);
	if (diff < EPSILON) break;
	}
}
