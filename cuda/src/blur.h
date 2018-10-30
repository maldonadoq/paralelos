#ifndef _BLUR_H_
#define _BLUR_H_

#include "pixel.h"

#define blur_size 3

__global__
void blur_kernel(pixel *in, pixel *out, unsigned w, unsigned h){
    unsigned col = threadIdx.x+(blockIdx.x*blockDim.x);
	unsigned row = threadIdx.y+(blockIdx.y*blockDim.y);
    
    if(col<w and row<h){
    	float valr = 0.0;
        float valg = 0.0;
        float valb = 0.0;
        float size = 0;

        int crow, ccol;
        for(int brow=-blur_size; brow<blur_size+1; brow++){
            for(int bcol=blur_size; bcol<blur_size+1; bcol++){
                crow = row+brow;
                ccol = col+bcol;

                if(crow>-1 and crow<h && ccol>-1 and ccol<w){
                    valr += in[(crow*w)+ccol].get_r();
                    valg += in[(crow*w)+ccol].get_g();
                    valb += in[(crow*w)+ccol].get_b();
                    size+=1;
                }
            }
        }

        out[(row*w)+col].set_r(valr/size);
        out[(row*w)+col].set_g(valg/size);
        out[(row*w)+col].set_b(valb/size);
    }
}

void kblur(pixel *h_in, pixel *h_gray, unsigned col, unsigned row){
    unsigned size = col*row*sizeof(pixel);

    pixel *d_in;
    pixel *d_out;

    cudaMalloc((void **)&d_in, size);
	cudaMalloc((void **)&d_out, size);

	cudaMemcpy(d_in, h_in, size, cudaMemcpyHostToDevice);

	unsigned block = 30;
	dim3 dimGrid(ceil(col/block), ceil(row/block), 1);
	dim3 dimBlock(block, block, 1);
	blur_kernel<<<dimGrid, dimBlock>>>(d_in, d_out, col, row);

	cudaMemcpy(h_gray, d_out, size, cudaMemcpyDeviceToHost);

	cudaFree(d_in);
	cudaFree(d_out);
}

#endif