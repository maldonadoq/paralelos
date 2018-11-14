#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "src/matrix.h"
#include "../src/print.h"

int main(int argc, char const *argv[]){	
	unsigned width = 1024;
	unsigned block = 32;

	unsigned ntotal = width*width;	

	int *h_m = new int[ntotal];
	int *h_n = new int[ntotal];
	int *h_p = new int[ntotal];
	int *h_r = new int[ntotal];

	unsigned i, j;
	for(i=0; i<width; i++){
		for(j=0; j<width; j++){
		    h_m[(i*width)+j] = j;
		    h_n[(i*width)+j] = j;
		    h_p[(i*width)+j] = 0;
		    h_r[(i*width)+j] = 0;
		}
	}
		
	struct timeval ti, tf;
	double time;

	gettimeofday(&ti, NULL);
		square_matrix_mult(h_m, h_n, h_p, width, block, 'n');
	gettimeofday(&tf, NULL);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("[%ix%i] global memory time: %.8lf s\n", width, width, time/1000);
	// print_matrix(h_p, width, width);

	gettimeofday(&ti, NULL);
		square_matrix_mult(h_m, h_n, h_r, width, block, 't');
	gettimeofday(&tf, NULL);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("[%ix%i] shared memory time: %.8lf s\n", width, width, time/1000);
	// print_matrix(h_r, width, width);

	delete h_m;
	delete h_n;
	delete h_p;
	delete h_r;

	return 0;
}

/*
	El número de tile_width en el archivo src/matrix.h esta definido por el tamaño de bloques,
	en si mi maquina soporta 1024 threads por bloque, y por cada thread usa un elemento de la 
	matriz en shared memory, por lo tanto cada matrix en shared memory debe ser del mismo tamaño
	que el bloque, tile_width = block" 

compile:
	nvcc square_matrix_mult.cu -o square_matrix_mult.out
run:
	./square_matrix_mult.out
*/
