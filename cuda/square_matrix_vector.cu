#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "src/matrix.h"
#include "../src/print.h"

int main(int argc, char const *argv[]){	
	unsigned width = 256;
	unsigned block = 16;

	unsigned ntotal = width*width;	

	int *h_m = new int[ntotal];
	int *h_v = new int[width];
	int *h_r = new int[width];

	unsigned i, j;
	for(i=0; i<width; i++){
		h_r[i] = 0;
		h_v[i] = 2;
		for(j=0; j<width; j++){
		    h_m[(i*width)+j] = 2;
		}
	}
		
	struct timeval ti, tf;
	double time;

	gettimeofday(&ti, NULL);
		square_matrix_vector_mult(h_m, h_v, h_r, width, block);
	gettimeofday(&tf, NULL);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("[%ix%i] time: %.8lf s\n", width, width, time/1000);
	print_vector(h_r, width);

	delete h_m; delete h_v;
	delete h_r;

	return 0;
}