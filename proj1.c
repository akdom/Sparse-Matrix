//schrist && ahkeslin
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DELTA_T(time) ( (clock()-time)/(double)CLOCKS_PER_SEC )
#define EPSILON 1e-6
#define ITERMAX 100

typedef struct Entry Entry;
struct Entry {
    unsigned long col;
    double val;
};

void get_entry(Entry *entry, FILE *input_file);
void get_dimensions(long *dim, long *entries, FILE *input_file);
int row_sort(const void *_a, const void *_b);
int col_sort(const void *_a, const void *_b);
void right_mult(Entry matrix[], long entries, Entry *rows[], long dim, double vector[], double out[]);
void left_mult(Entry matrix[], long entries, Entry *rows[], long dim, double vector[], double out[]);
void normalize(double vector[], long dim);
double vect_diff(double a[], double b[], long dim);
int calc_eigenvector(Entry matrix[], long entries, Entry *rows[], long dim, void (*mult)(Entry matrix[], long entries, Entry *rows[], long dim, double vector[], double out[]));

int main (int argc, char *argv[]) {
    long dim, entries;
    clock_t time = clock();
    int iterations;
    
    char *filename = argv[1];
    if (filename == NULL) {
        printf("THE END OF THE WORLD IS UPON US... THE ONE TRUE FILE DOES NOT EXIST!!! (or hasn't been handed as the first argument...)");
        exit(EXIT_FAILURE);
    }
    
    FILE *input_file = fopen(filename, "r");
    get_dimensions(&dim, &entries, input_file);
    
    //Create our matrix
    Entry matrix[entries];
    Entry *rows[dim+1];
    rows[dim]=&matrix[entries];
        
    int count;
    Entry *e, *last=&matrix[entries-1];
    long unsigned int cur_row = 0, prev_row = 0;
    
    for(e=matrix; e<=last; e++) {
        prev_row = cur_row;
        count = fscanf(input_file, "%lf %lu %lu", &(e->val), &(e->col), &cur_row);
        if (count != 3) {
            perror("The data file has a corrupt line\n");
        }
        if (cur_row != prev_row) {
            rows[cur_row-1] = e;
        }

        (e->col)--;
    }
    
    printf("Time to load matrix: %lf\n", DELTA_T(time));
    
    fclose(input_file);
    
    time=clock();
    iterations = calc_eigenvector(matrix, entries, rows, dim, &left_mult);
    printf("Time to compute left eigenvector: %lf; iterations: %d;\n", DELTA_T(time), iterations);
    
    time=clock();
    iterations =  calc_eigenvector(matrix, entries, rows, dim, &right_mult);
    printf("Time to compute right eigenvector: %lf; iterations: %d;\n", DELTA_T(time), iterations); 
    
    return (0);
}

void get_dimensions(long *dim, long *entries, FILE *input_file) {
    int count = fscanf(input_file, "%ld %ld", dim, entries);
    if(count != 2) {
        perror("The input file does not state the dimension and number of entries on the first line.\n");
    }
}

void right_mult(Entry matrix[], long entries, Entry *rows[], long dim, double vector[], double out[]) {
    Entry *e;
    long i;
    
    for(i=0; i<dim; i++) {
        for(e=rows[i]; e<rows[i+1]; e++) {
            out[i] += e->val * vector[e->col];
        }
    }
}

void left_mult(Entry matrix[], long entries, Entry *rows[], long dim, double vector[], double out[]) {
    Entry *e;
    int i;
    for(i=0; i<dim; i++) {
        for(e=rows[i]; e<rows[i+1]; e++) {
            out[e->col] += e->val * vector[i];
        }
    }
}

void normalize(double vector[], long dim) {
    double *cur, *last, total=0;
    last = &vector[dim-1];
    for(cur=vector; cur<=last; cur++) {
        total += *cur;
    }
    for(cur=vector; cur<=last; cur++) {
        *cur /= total;
    }
}

double vect_diff(double a[], double b[], long dim) {
    double *cur_a, *cur_b, *last_a, total=0;
    last_a = &a[dim-1];
    for(cur_a=a, cur_b=b; cur_a<=last_a; cur_a++, cur_b++) {
        total += ((*cur_a - *cur_b)>0 ? (*cur_a - *cur_b) : (*cur_b - *cur_a));
    }
    return total;
}

int calc_eigenvector(Entry matrix[], long entries, Entry *rows[], long dim, void (*mult)(Entry matrix[], long entries, Entry *rows[], long dim, double vector[], double out[])) {
    double vector1[dim], vector2[dim];
    double *temp, *source, *dest, *d, diff;

    int i;
    for(i=0; i < dim; i++) {
        vector1[i]= 1.0 * ((i+1) % 2);
    }
    // start them in the "wrong" order, so that the first swap corrects it
    dest = vector1;
    source = vector2;
    
    normalize(dest, dim);
    
    for(i=0; i<ITERMAX; i++) {
        //swap the vectors
        temp = source;
        source = dest;
        dest = temp;
        for(d = &dest[dim-1]; d>=dest; d--) { *d = 0.0; } //clear dest 
        
        mult(matrix, entries, rows, dim, source, dest);
        
        normalize(dest, dim);
        
        diff = vect_diff(dest, source, dim);
        printf("i: %d; |x-%c|: %lg\n", i, (mult==&left_mult ? 'y' : 'z'), diff);
        if (diff < EPSILON) break;
    }
    
    if (i != ITERMAX) { i += 1; }
    return i;
}
