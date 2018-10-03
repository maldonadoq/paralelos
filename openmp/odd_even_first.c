#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int thc;

void odd_even(int *a, int n){
    int phase, i, tmp;

    for(phase = 0; phase < n; phase++){
        if(phase % 2 == 0){
            #pragma omp parallel for num_threads(thc) default(none) shared(a,n) private(i,tmp)
                for(i=1; i<n; i+=2){
                    if(a[i-1] > a[i]){
                       tmp = a[i-1];
                       a[i-1] = a[i];
                       a[i] = tmp;
                    }
                }
        }
        else{
            #pragma omp parallel for num_threads(thc) default(none) shared(a,n) private(i,tmp)
                for(i=1; i<n-1; i+=2){
                    if (a[i] > a[i+1]){
                       tmp = a[i+1];
                       a[i+1] = a[i];
                       a[i] = tmp;
                    }
                }
        }
    }
}

void print(int *a, int n){
    for(int i=0; i<n; i++)
        printf("%d ", a[i]);
    printf("\n");
}

int main(int argc, char* argv[]) {
    int  n;
    int* a;
    double start, finish;

    n = 5000;
    a = malloc(n*sizeof(int));
   
    int i;
    srand(time(NULL));
    for(i=0; i<n; i++)
        a[i] = rand()%n;

    start = omp_get_wtime();
    odd_even(a,n);
    finish = omp_get_wtime();

    //print(a,n);
    printf("time = %.6lf s\n", finish-start);

    free(a);
    return 0;
}