#ifndef _VECTOR_H_
#define _VECTOR_H_

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

#endif