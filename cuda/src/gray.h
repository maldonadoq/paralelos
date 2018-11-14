#ifndef _GRAY_H_
#define _GRAY_H_

#include "pixel.h"

__global__
void gray_scale_kernel(pixel *in, float *gray, unsigned col, unsigned row){
    unsigned c = threadIdx.x+(blockIdx.x*blockDim.x);
	unsigned r = threadIdx.y+(blockIdx.y*blockDim.y);
    
    if(c<col and r<row){    	
    	unsigned id = (r*col)+c;
    	gray[id] = (0.299*in[id].get_r())+
                    (0.587*in[id].get_g())+
                    (0.114*in[id].get_b());
    }
}

void gray_scale(pixel *in, float *h_out, float col, float row, float block){
    unsigned msize = (unsigned)col*row*sizeof(pixel);     // pixel vector
    unsigned rsize = (unsigned)col*row*sizeof(float);     // float vector

    pixel *d_in;
    float *d_out;

    cudaMalloc((void **)&d_in, msize);
	cudaMalloc((void **)&d_out, rsize);

	cudaMemcpy(d_in, in, msize, cudaMemcpyHostToDevice);
    
	dim3 dimGrid(ceil(col/block), ceil(row/block), 1);
	dim3 dimBlock(block, block, 1);

	gray_scale_kernel<<<dimGrid, dimBlock>>>(d_in, d_out, col, row);

	cudaMemcpy(h_out, d_out, rsize, cudaMemcpyDeviceToHost);

	cudaFree(d_in);
	cudaFree(d_out);
}

#endif