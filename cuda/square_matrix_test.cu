#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "src/test.h"
#include "../src/print.h"

int main(int argc, char const *argv[]){	
	unsigned width = 1024;
	unsigned block = 32;

	unsigned ntotal = width*width;	

	int *h_m = new int[ntotal];
	int *h_n = new int[ntotal];
	int *h_p = new int[ntotal];
	int *h_q = new int[ntotal];
	int *h_r = new int[ntotal];

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
		square_matrix_sum_test(h_m, h_n, h_p, width, block,'B');		
	gettimeofday(&tf, NULL);
	//print_matrix(h_p, width, width);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("B time: %.8lf s\n", time/1000);

	gettimeofday(&ti, NULL);	
		square_matrix_sum_test(h_m, h_n, h_q, width, block,'C');		
	gettimeofday(&tf, NULL);
	//print_matrix(h_q, width, width);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("C time: %.8lf s\n", time/1000);

	gettimeofday(&ti, NULL);	
		square_matrix_sum_test(h_m, h_n, h_r, width, block,'D');		
	gettimeofday(&tf, NULL);
	//print_matrix(h_r, width, width);
	time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000;
	printf("D time: %.8lf s\n", time/1000);

	delete h_m; delete h_n;
	delete h_p;
	return 0;
}