#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NOW() ({ struct timespec ts; clock_gettime(CLOCK_MONOTONIC, &ts); ts; })
#define TIME(start, end) ((double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_nsec - start.tv_nsec) / 1000000.0)

double* allocate_array(int len) {
    return (double*)malloc(len * sizeof(double));
}

void fill_array_with_random_values(double* matrix, int len) {
    for (int i = 0; i < len; i++) {
        matrix[i] = (double)rand() / RAND_MAX;
    }
}

void selection_sort(double* array_a, int len) {
    struct timespec start, end;
	start = NOW();

    int index=0;
    double tmp;

    for(int j=0;j<len-1;j++){
        for(int i=0;i<len-j;i++){
            if(array_a[i] > array_a[index]){
                index = i;
            }
        }
        tmp = array_a[index];
        array_a[index] = array_a[len-j-1];
        array_a[len-j-1] = tmp;
        index = 0;
    }

    end = NOW();
	double elapsed = TIME(start, end);
    printf("%d,%lf\n",len,elapsed);
    //printf("Array Length: %d\n",len);
    //printf("Total Elapsed Time: %f ms\n", elapsed);
}


void print_array(double* matrix, int len) {
    for (int i = 0; i < len; i++) {
            printf("%lf ", matrix[i]);
    }
    printf("\n");
}

int main(int argc,char *argv[]) {
    int t = atoi(argv[1]);
        
        srand(time(NULL)); 

        int len = t;
        
        double* array = allocate_array(len);

        fill_array_with_random_values(array, len);

        selection_sort(array, len);

        // Print the matrices
        /*printf("Matrix 1:\n");
        print_matrix(matrix1, rows, cols);

        printf("\nMatrix 2:\n");
        print_matrix(matrix2, rows, cols);

        printf("\nResult (Matrix 1 + Matrix 2):\n");
        print_matrix(result, rows, cols);
        */

        free(array);
        

    return 0;
}
