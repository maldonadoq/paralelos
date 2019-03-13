#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>

double add(int *vect, int n){
    int my_sum = 0;
    int  i, local_n;

    int my_rank = omp_get_thread_num();
    int thc = omp_get_num_threads();

    local_n = n/thc;
    int in = my_rank*local_n;
    int en = in+local_n;

    for (i=in; i<en; i++){
        my_sum += vect[i];
    }

    return my_sum;
}


int main(int argc, char const *argv[]){
    int sum = 0.0;
    int tam = 10;
    int vect[tam];
    int thc, i;

    thc = 10;

    for(i=0; i<tam; i++)    vect[i] = i;

    #pragma omp parallel num_threads(thc) reduction(+: sum)
        sum += add(vect,tam);

    printf("sum: %d\n", sum);

    return 0;
   
}