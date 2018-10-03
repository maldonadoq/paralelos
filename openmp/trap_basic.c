#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

double f(double x){ return x*x; }

void trap(double a, double b, int n, double* result_p) {
    double  h, x, my_result;
    double  local_a, local_b;
    int  i, local_n;
    int my_rank = omp_get_thread_num();
    int thc = omp_get_num_threads();

    h = (b-a)/n; 
    
    local_n = n/thc;  
    local_a = a + my_rank*local_n*h; 
    local_b = local_a + local_n*h; 
    my_result = (f(local_a) + f(local_b))/2.0; 
    
    for (i = 1; i <= local_n-1; i++) {
        x = local_a + i*h;
        my_result += f(x);
    }
    
    my_result = my_result*h; 

    //#pragma omp critical 
        *result_p += my_result; 
}

int main(int argc, char const *argv[]){
    double  result = 0.0;
    double a, b;
    long long n;
    int thc;


    a = -10;
    b = 10;
    n = 100000000;

    for(thc=2; thc<10; thc++){
        // double start_time = omp_get_wtime();    

        result = 0;
        #pragma omp parallel num_threads(thc)
            trap(a, b, n, &result);        
        //#pragma omp barrier

        //double end_time = omp_get_wtime();
        printf("thc %d\tintegral: %.16lf\n", thc, result);   
        //printf("time: %.6lf s\n", end_time-start_time);
    }
    return 0;
}

/*
th 2    integral: 666.6666666666827723
th 3    integral: 666.6666466666885071
th 4    integral: 666.6666666667144909
th 5    integral: 666.6666666667570098
th 6    integral: 666.6665866667234468
th 7    integral: 666.6666266667109539
th 8    integral: 666.6666666667138088
th 9    integral: 666.6666466666957831
th 10   integral: 666.6666666666732226
*/