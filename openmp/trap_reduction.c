#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

double f(double x){ return x*x; }

double local_trap(double a, double b, int n) {
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

    return my_result*h;
}


int main(int argc, char const *argv[]){
    double  result = 0.0;
    double a, b;
    long long n;
    int thc;

    a = -10;
    b = 10;
    n = 100000000;
    thc = 2;   

    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(thc) reduction(+: result)
        result += local_trap(a, b, n);

    #pragma omp barrier

    double end_time = omp_get_wtime();
    printf("integral: %.8lf\n", result);   
    printf("time: %.6lf s\n", end_time-start_time);

    return 0;
   
}