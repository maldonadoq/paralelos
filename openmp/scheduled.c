#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>


int main(int argc, char const *argv[]){
	int i, thc, ni;
	thc = 2;
	ni = 4;
	#pragma omp parallel for schedule(dynamic) num_threads(thc)
		for(i=0; i<ni; i++){
			printf("Thread %d: Iterations %d.\n", omp_get_thread_num(), i);
		}

	return 0;
}