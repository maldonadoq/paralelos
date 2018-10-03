#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

double f(double x){ return x*x; }

double trap(double a, double b, int n, int thc){
    double  h, approx;
    int  i;

    h = (b-a)/n; 
    approx = (f(a) + f(b))/2.0; 
    #pragma omp parallel for num_threads(thc) reduction(+: approx)
        for (i = 1; i <= n-1; i++)
            approx += f(a + i*h);
        approx = h*approx;

    return approx;
}

int main(int argc, char const *argv[]){
    double  result = 0.0;
    double  a, b;
    long long n;
    int thc;


    a = -10;
    b = 10;
    n = 100000;
    thc = 2;


    double start_time = omp_get_wtime();

    result = trap(a, b, n, thc);

    #pragma omp barrier

    double end_time = omp_get_wtime();
    printf("integral: %.8lf\n", result);   
    printf("time: %.6lf s\n", end_time-start_time);
    return 0;
}