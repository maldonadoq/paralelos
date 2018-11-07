#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "../src/print.h"
#include "src/vector.h"

int main(int argc, char const *argv[]){
	unsigned n = 100;
	unsigned thc = 25;

	int *h_a = new int[n];
	int *h_b = new int[n];
	int *h_c = new int[n];

	unsigned i;
	for(i=0; i<n; i++){
		h_a[i] = i;
		h_b[i] = i;
		h_c[i] = 0;
	}

	vect_add(h_a, h_b, h_c, n, thc);

	print_vector(h_c, n);

	delete h_a;
	delete h_b;
	delete h_c;

	return 0;
}