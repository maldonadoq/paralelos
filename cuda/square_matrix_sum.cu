#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "src/matrix.h"
#include "../src/print.h"

int main(int argc, char const *argv[]){	
	unsigned width = 20;
	unsigned block = 16;

	unsigned ntotal = width*width;	

	int *h_m = new int[ntotal];
	int *h_n = new int[ntotal];
	int *h_p = new int[ntotal];

	unsigned i, j;
	for(i=0; i<width; i++){
		for(j=0; j<width; j++){
		    h_m[(i*width)+j] = j;
		    h_n[(i*width)+j] = j;
		    h_p[(i*width)+j] = 0;
		}
	}
		
	struct timeval ti, tf;
	double time;

	gettimeofday(&ti, NULL);
		square_matrix_sum(h_m, h_n, h_p, width, block);
	gettimeofday(&tf, NULL);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("[%ix%i] time: %.8lf s\n", width, width, time/1000);	

	print_matrix(h_p, width, width);

	delete h_m; delete h_n;
	delete h_p;
	return 0;
}