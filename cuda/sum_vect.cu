#include <iostream>
#include <stdlib.h>
#include <stdio.h>

void print(int *v, unsigned s){
	for(unsigned i=0; i<s; i++)	std::cout << v[i] << " ";
	std::cout << "\n";
}

__global__
void vect_add_kernel(int *a, int *b, int *c, int n){
	unsigned i = (blockDim.x*blockIdx.x)+threadIdx.x;

	if(i<n)	c[i] = a[i]+b[i];
}

void vect_add(int *a, int *b, int *c, unsigned n, unsigned thc){
	unsigned size = n*sizeof(int);
	int *d_a, *d_b, *d_c;

	//print(a,n);
	//print(b,n);

	cudaMalloc((void **)&d_a, size);
	cudaMalloc((void **)&d_b, size);
	cudaMemcpy(d_a, a, size, cudaMemcpyHostToDevice);	
	cudaMemcpy(d_b, b, size, cudaMemcpyHostToDevice);

	cudaMalloc((void **)&d_c, size);
	vect_add_kernel<<<ceil(n/thc), thc>>>(d_a, d_b, d_c, n);

	cudaMemcpy(c, d_c, size, cudaMemcpyDeviceToHost);

	cudaFree(d_a);
	cudaFree(d_b);
	cudaFree(d_c);
}

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

	print(h_c, n);

	delete h_a;
	delete h_b;
	delete h_c;

	return 0;
}