#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

void mat_vect(int *A, int *x, int *y, int m, int n, int thc){
    int i, j;    
    #pragma omp parallel for num_threads(thc) default(none) private(i, j)  shared(A, x, y, m, n)
    for(i = 0; i<m; i++){
        y[i] = 0.0;
        for (j=0; j<n; j++)
            y[i] += A[(i*n)+j]*x[j];
    }
}

void print(int *y, int m){
    for (int i=0; i<m; i++)
        printf("%d ", y[i]);
    printf("\n");
}

int main(int argc, char const *argv[]){
    int thc, m, n;
    int *A, *x, *y;

    thc = 4;
    m = 8000;
    n = 8000;

    A = malloc(m*n*sizeof(int));
    x = malloc(n*sizeof(int));
    y = malloc(m*sizeof(int));

    int i,j;
    srand(time(NULL));
    // rand matrix
    for(i=0; i<m; i++){
        for(j=0; j<n; j++)
            A[(i*n)+j] = rand()%m;
    }

    // rand vector
    for(i=0; i<n; i++)
        x[i] = rand()%n;


    double start, finish;

    start = omp_get_wtime();
    mat_vect(A, x, y, m, n, thc);
    finish = omp_get_wtime();

    printf("time = %.6lf s\n", finish-start);
    //print(y,m);

    free(A); free(x); free(y);
    return 0;
}

