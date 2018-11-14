#ifndef _TEST_H_
#define _TEST_H_

// function kernel to sum
__global__
void square_matrix_sum_kernelB(int *m, int *n, int *p, unsigned width){
	unsigned col = threadIdx.x+(blockIdx.x*blockDim.x);
	unsigned row = threadIdx.y+(blockIdx.y*blockDim.y);

	if(col<width and row<width){
		p[(row*width)+col] = m[(row*width)+col]+n[(row*width)+col];
	}
}

__global__
void square_matrix_sum_kernelC(int *m, int *n, int *p, unsigned width){
	unsigned idx = threadIdx.x+(blockIdx.x*blockDim.x);

	if(idx<width){
		unsigned r;
		for(unsigned i=0; i<width; i++){
			r = (idx*width)+i;
			p[r] = m[r]+n[r];
		}
	}
}

__global__
void square_matrix_sum_kernelD(int *m, int *n, int *p, unsigned width){
	unsigned idx = threadIdx.x+(blockIdx.x*blockDim.x);

	if(idx<width){
		unsigned r;
		for(unsigned i=0; i<width; i++){
			r = (i*width)+idx;
			p[r] = m[r]+n[r];
		}
	}
}

// function host to call device kernel_mult
void square_matrix_sum_test(int *m, int *n, int *p, float width, float block, char type){
	unsigned size = width*width*sizeof(int);
	int *d_m, *d_n, *d_p;

	cudaMalloc((void **)&d_m, size);
	cudaMalloc((void **)&d_n, size);
	cudaMalloc((void **)&d_p, size);

	cudaMemcpy(d_m, m, size, cudaMemcpyHostToDevice);	
	cudaMemcpy(d_n, n, size, cudaMemcpyHostToDevice);	

	switch(type){
		case 'B':{
			dim3 dimGrid(ceil(width/block), ceil(width/block), 1);
			dim3 dimBlock(block, block, 1);	
			square_matrix_sum_kernelB <<<dimGrid, dimBlock>>>(d_m, d_n, d_p, width);			
			break;
		}
		case 'C':{
			square_matrix_sum_kernelC <<<ceil(width/block), block>>>(d_m, d_n, d_p, width);
			break;
		}
		case 'D':{
			square_matrix_sum_kernelD <<<ceil(width/block), block>>>(d_m, d_n, d_p, width);
			break;
		}
		default:
			break;
	}

	cudaMemcpy(p, d_p, size, cudaMemcpyDeviceToHost);

	cudaFree(d_m);
	cudaFree(d_n);
	cudaFree(d_p);
}

#endif