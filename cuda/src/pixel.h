#ifndef _PIXEL_H_
#define _PIXEL_H_

class pixel{
public:
    float r,g,b;
    __host__ __device__ pixel(float _r, float _g, float _b){
        this->r = _r;
        this->g = _g;
        this->b = _b;
    }

    __host__ __device__ pixel(){
        this->r = 0.0;
        this->g = 0.0;
        this->b = 0.0;
    }

    __host__ __device__ void set_r(float _r){   this->r = _r;   }
    __host__ __device__ void set_g(float _g){   this->g = _g;   }
    __host__ __device__ void set_b(float _b){   this->b = _b;   }

    __host__ __device__ float get_r(){	return this->r;	};
    __host__ __device__ float get_g(){	return this->g;	};
    __host__ __device__ float get_b(){	return this->b;	};
};

#endif