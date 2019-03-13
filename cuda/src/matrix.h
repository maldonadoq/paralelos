#ifndef _MATRIX_H_
#define _MATRIX_H_

#define tile_width 32


// kernel function to normal_square_matrix_mult_kernel
__global__
void normal_square_matrix_mult_kernel(int *m, int *n, int *p, unsigned width){
	unsigned col = threadIdx.x+(blockIdx.x*blockDim.x);
	unsigned row = threadIdx.y+(blockIdx.y*blockDim.y);

	if(col<width and row<width){
		int pvalue = 0;
		for(unsigned k=0; k<width; k++){
			pvalue += m[(row*width)+k]*n[(k*width)+col];
		}

		p[(row*width)+col] = pvalue;
	}
}

// kernel function to tile_square_matrix_mult_kernel
__global__
void tile_square_matrix_mult_kernel(int *m, int *n, int *p, unsigned width){
	__shared__ int mds[tile_width][tile_width];
	__shared__ int nds[tile_width][tile_width];

	unsigned bx = blockIdx.x;
	unsigned by = blockIdx.y;

	unsigned tx = threadIdx.x;
	unsigned ty = threadIdx.y;

	unsigned row = (by*tile_width)+ty;	
	unsigned col = (bx*tile_width)+tx;

	int pvalue = 0;
	unsigned ph, k;
	for(ph=0; ph<width/tile_width; ph++){
		mds[ty][tx] = m[(row*width)+(ph*tile_width) + tx];
		nds[ty][tx] = n[(((ph*tile_width)+ty)*width) + col];
		__syncthreads();

		for(k=0; k<tile_width; k++){
			pvalue += mds[ty][k]*nds[k][tx];
		}
		__syncthreads();
	}

	p[(row*width)+col] = pvalue;
}

// function host to call device kernel_mult
void square_matrix_mult(int *m, int *n, int *p, float width, float block, char type){
	unsigned size = width*width*sizeof(int);
	int *d_m, *d_n, *d_p;

	cudaMalloc((void **)&d_m, size);
	cudaMalloc((void **)&d_n, size);
	cudaMalloc((void **)&d_p, size);

	cudaMemcpy(d_m, m, size, cudaMemcpyHostToDevice);	
	cudaMemcpy(d_n, n, size, cudaMemcpyHostToDevice);	

	dim3 dimGrid(ceil(width/block), ceil(width/block), 1);
	dim3 dimBlock(block, block, 1);

	switch(type){
		case 'n':
			normal_square_matrix_mult_kernel<<<dimGrid, dimBlock>>>(d_m, d_n, d_p, width);
			break;
		case 't':
			tile_square_matrix_mult_kernel<<<dimGrid, dimBlock>>>(d_m, d_n, d_p, width);
			break;
		default:
			std::cout << "  type [0]-[1]!!!";
			break;
	}	

	cudaMemcpy(p, d_p, size, cudaMemcpyDeviceToHost);

	cudaFree(d_m);
	cudaFree(d_n);
	cudaFree(d_p);
}

// function kernel to sum
__global__
void square_matrix_sum_kernel(int *m, int *n, int *p, unsigned width){
	unsigned col = threadIdx.x+(blockIdx.x*blockDim.x);
	unsigned row = threadIdx.y+(blockIdx.y*blockDim.y);

	if(col<width and row<width){
		p[(row*width)+col] = m[(row*width)+col]+n[(row*width)+col];
	}
}

// function host to call device kernel_mult
void square_matrix_sum(int *m, int *n, int *p, float width, float block){
	unsigned size = width*width*sizeof(int);
	int *d_m, *d_n, *d_p;

	cudaMalloc((void **)&d_m, size);
	cudaMalloc((void **)&d_n, size);
	cudaMalloc((void **)&d_p, size);

	cudaMemcpy(d_m, m, size, cudaMemcpyHostToDevice);	
	cudaMemcpy(d_n, n, size, cudaMemcpyHostToDevice);	

	dim3 dimGrid(ceil(width/block), ceil(width/block), 1);
	dim3 dimBlock(block, block, 1);

	
	square_matrix_sum_kernel<<<dimGrid, dimBlock>>>(d_m, d_n, d_p, width);			

	cudaMemcpy(p, d_p, size, cudaMemcpyDeviceToHost);

	cudaFree(d_m);
	cudaFree(d_n);
	cudaFree(d_p);
}

// matrix - vector
__global__
void square_matrix_vector_mult_kernel(int *m, int *v, int *p, unsigned width){
	unsigned idx = threadIdx.x+(blockIdx.x*blockDim.x);
	if(idx<width){		
		for(unsigned i=0; i<width; i++){
			p[idx] += m[(idx*width)+i]*v[i];
		}
	}
}

// function host to call device kernel_mult
void square_matrix_vector_mult(int *m, int *v, int *p, float width, float block){
	unsigned msize = width*width*sizeof(int);
	unsigned vsize = width*sizeof(int);
	int *d_m, *d_v, *d_p;

	cudaMalloc((void **)&d_m, msize);
	cudaMalloc((void **)&d_v, vsize);
	cudaMalloc((void **)&d_p, vsize);

	cudaMemcpy(d_m, m, msize, cudaMemcpyHostToDevice);	
	cudaMemcpy(d_v, v, vsize, cudaMemcpyHostToDevice);	
	
	square_matrix_vector_mult_kernel<<<ceil(width/block), block>>>(d_m, d_v, d_p, width);

	cudaMemcpy(p, d_p, vsize, cudaMemcpyDeviceToHost);

	cudaFree(d_m);
	cudaFree(d_v);
	cudaFree(d_p);
}

#endif