#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL/cl.h>

#define 	MEM_SIZE                (128)
#define 	MAX_SOURCE_SIZE 	(0x100000)	

void print_vec(int *vec, int ts) {
	for (int i = 0; i < ts; i++) {
		printf("%d ", vec[i]);
	}
	printf("\n");
}

int main(void) {
	long int ts = 100;

	int *a, *b, *c;
	size_t data_size = ts*sizeof(int);

	a = (int *)malloc(data_size);
	b = (int *)malloc(data_size);
	c = (int *)malloc(data_size);	

	unsigned i;
	for(i=0; i<ts; i++){
		a[i] = i;
		b[i] = i;
		c[i] = 0;
	}

	// gpu vector add
	cl_mem a_dev, b_dev, c_dev;
	a_dev = NULL;
	b_dev = NULL;
	c_dev = NULL;

	cl_platform_id platform_id = NULL;
	cl_uint ret_num_platforms;

	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;

	cl_context context = NULL;
	cl_kernel kernel = NULL;
	cl_program program = NULL;

	cl_command_queue command_queue = NULL;
	cl_int ret;

	// kernel source
	char string[MEM_SIZE];
	FILE *fp;
	char fileName[] = "./vect_add.cl";
	char *source_str;
	size_t source_size;

	fp = fopen(fileName, "r");
	if(!fp){		
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*) malloc (MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// platform
	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	if(ret != CL_SUCCESS){
	    printf("Failed to get platform ID.\n");
		goto error;
	}

	// device
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
	if(ret != CL_SUCCESS){
	    printf("Failed to get device ID.\n");
		goto error;
	}
	// context
	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);
	if(ret != CL_SUCCESS){
		printf("Failed to create OpenCL context.\n");
	    goto error;
	}

	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	if(ret != CL_SUCCESS){
	   printf("Failed to create command queue %d\n", (int) ret);
	   goto error;
	}

	// memory buffer
	a_dev = clCreateBuffer (context, CL_MEM_READ_ONLY, data_size, NULL, &ret);
	b_dev = clCreateBuffer (context, CL_MEM_READ_ONLY, data_size, NULL, &ret);
	c_dev = clCreateBuffer (context, CL_MEM_WRITE_ONLY, data_size, NULL, &ret);
	
	ret = clEnqueueWriteBuffer (command_queue, a_dev, CL_TRUE, 0, data_size, (void *)a, 0, NULL, NULL);
	ret |= clEnqueueWriteBuffer (command_queue, b_dev, CL_TRUE, 0, data_size, (void *)b, 0, NULL, NULL);
	if(ret != CL_SUCCESS){
		printf("Failed to copy date from host to device: %d\n", (int) ret);
		goto error;
	}
	
	// create kernel program
	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create OpenCL program from source %d\n", (int) ret);
		goto error;
	}

	// build kernel program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	if(ret != CL_SUCCESS){
		printf("Failed to build program %d\n", (int) ret);
		char build_log[16348];
		clGetProgramBuildInfo (program, device_id, CL_PROGRAM_BUILD_LOG, sizeof (build_log), build_log, NULL);
		printf ("Error in kernel: %s\n", build_log);
		goto error;
	}

	// create OpenCL kernel
	kernel = clCreateKernel(program, "add_vec_gpu", &ret);
	if(ret != CL_SUCCESS){
		printf("Failed to create kernel %d\n", (int) ret);
		goto error; 
	}

	ret  = clSetKernelArg(kernel, 0, sizeof (cl_mem), (void *) &a_dev);
	ret |= clSetKernelArg(kernel, 1, sizeof (cl_mem), (void *) &b_dev);
	ret |= clSetKernelArg(kernel, 2, sizeof (cl_mem), (void *) &c_dev);
	ret |= clSetKernelArg(kernel, 3, sizeof (cl_int), (void *) &ts);
	if(ret != CL_SUCCESS){
		printf("Failed to set kernel arguments %d\n", (int) ret);
		goto error;
	}

    size_t global_work_size, local_work_size;  
    local_work_size = ts;  
    // Number of total work items - localSize must be devisor  
    global_work_size = (size_t)ceil(ts/(float)local_work_size)*local_work_size;

	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_work_size, &local_work_size, 0, NULL, NULL);
	if(ret != CL_SUCCESS){
		printf("Failed to execute kernel for execution %d\n", (int) ret);
		goto error;
	}

	// copy results from the memory buffer
	ret = clEnqueueReadBuffer(command_queue, c_dev, CL_TRUE, 0, data_size, (void *)c, 0, NULL, NULL);
	if (ret != CL_SUCCESS) {
		printf("Failed to copy data from device to host %d\n", (int) ret);
		goto error;
	}

	printf("c\t: ");	print_vec(c, ts);

error:
    // free device resources
	clFlush(command_queue);
	clFinish(command_queue);
	clReleaseKernel(kernel);
	clReleaseProgram(program);

	clReleaseMemObject(a_dev);
	clReleaseMemObject(b_dev);
	clReleaseMemObject(c_dev);

	clReleaseCommandQueue(command_queue);
	clReleaseContext(context);

    // free host resources
	free(source_str);
	free(a);
	free(b);
	free(c);

	return 0;
}

// gcc vect_add.c -o vect_add.out -lOpenCL -lm