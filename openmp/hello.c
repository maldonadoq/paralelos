#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void hello(){
	int my_rank = omp_get_thread_num();
	int thc = omp_get_num_threads();
	printf("Hello from thread %d of %d\n", my_rank, thc);
}

int main(int argc, char const *argv[]){
	int thc = 10;
	
	#pragma omp parallel num_threads(thc)
		hello();
	return 0;
}