#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <CL/cl.h>

#define CHECK_ERROR(status, error_message) \
	check_error(status, __FILE__, __LINE__, error_message)

#define VECTOR_LENGTH 1024

double time(void)
{
	static int sec = -1;
	struct timeval tv;
	gettimeofday(&tv, NULL);

	if (sec < 0) sec = tv.tv_sec;
	return (tv.tv_sec - sec) + 1.0e-6 * tv.tv_usec;
}

void check_error(cl_int status, char *file, int line, char *error_message)
{
	if(status != CL_SUCCESS) {
		printf("%s:%d: error, %s\n", file, line, error_message);
		exit(0);
	}
}

const char *kernel_src =
"__kernel void vadd(__global float *a, __global float *b,        \n"
"                   __global float *c, const unsigned int count) \n"
"{                                                               \n"
"	int i = get_global_id(0);                                \n"
"	if(i < count)                                            \n"
"		c[i] = a[i] + b[i];                              \n"
"}                                                               \n";

int main(void)
{
	cl_int err;

	float *vec_a = (float *)calloc(VECTOR_LENGTH, sizeof(float));
	float *vec_b = (float *)calloc(VECTOR_LENGTH, sizeof(float));
	float *vec_c = (float *)calloc(VECTOR_LENGTH, sizeof(float));
	unsigned count = VECTOR_LENGTH;

	int i = 0;
	for(i = 0; i < VECTOR_LENGTH; i++) {
		vec_a[i] = rand() / (float)RAND_MAX;
		vec_b[i] = rand() / (float)RAND_MAX;
		vec_c[i] = rand() / (float)RAND_MAX;
	}

	/* check platform numbers */
	cl_uint platform_nums;
	err = clGetPlatformIDs(0, NULL, &platform_nums);
	if(platform_nums == 0) {
		printf("found 0 platforms!\n");
		return 0;
	}

	/* get all platform ids*/
	cl_platform_id platforms[platform_nums];
	err = clGetPlatformIDs(platform_nums, platforms, NULL);
	CHECK_ERROR(err, "bad return value from clGetPlatformIDs!");

	/* find for the gpu device */
	cl_device_id dev_id;
	for(i = 0; i < platform_nums; i++) {
		err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_GPU, 1, &dev_id, NULL);

		/* we use the first gpu to run the test program */
		if(err == CL_SUCCESS) {
			char s[1000];
			clGetDeviceInfo(dev_id, CL_DEVICE_NAME, sizeof(s), &s, NULL);
                        printf("found gpu: %s\n-\n", s);

			break;
		}
	}

	if(err != CL_SUCCESS) {
		printf("cannot find any gpu device\n");
		return 0;
	}

	/* create compute context */
	cl_context context = clCreateContext(0, 1, &dev_id, NULL, NULL, &err);
	CHECK_ERROR(err, "bad return value from clCreateContext!");

	/* create compute commands */
	cl_command_queue commands = clCreateCommandQueue(context, dev_id, 0, &err);
	CHECK_ERROR(err, "bad return value from clCreateCommandQueue!");

	/* create compute program */
	cl_program program =
		clCreateProgramWithSource(context, 1, (const char **)&kernel_src, NULL, &err);
	CHECK_ERROR(err, "bad return value from clCreateProgramWithSource!");

	/* build cl program */
	printf("start building cl program...\n-\n");
	err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if(err != CL_SUCCESS) {
		printf("error, failed to build cl program!\n");

		size_t len;
		char s[10000] = {0};
		clGetProgramBuildInfo(program,dev_id, CL_PROGRAM_BUILD_LOG, sizeof(s), s, &len);
		printf("%s\n", s);
	}

	/* create compute kernel from the program */
	cl_kernel kernel_vadd = clCreateKernel(program, "vadd", &err);
	CHECK_ERROR(err, "bad return value from clCreateKernel!");

	/* create the input (a, b) and output (c) in the device memory (gpu) */
	cl_mem d_a = clCreateBuffer(context, CL_MEM_READ_ONLY,
			            sizeof(float) * count, NULL, &err);
	CHECK_ERROR(err, "bad return value from clCreateBuffer!");

	cl_mem d_b = clCreateBuffer(context, CL_MEM_READ_ONLY,
			            sizeof(float) * count, NULL, &err);
	CHECK_ERROR(err, "bad return value from clCreateBuffer!");

	cl_mem d_c = clCreateBuffer(context, CL_MEM_READ_ONLY,
			            sizeof(float) * count, NULL, &err);
	CHECK_ERROR(err, "bad return value from clCreateBuffer!");

	/* write a, b and c vectors into device memory */
	err = clEnqueueWriteBuffer(commands, d_a, CL_TRUE, 0, sizeof(float) * count,
				   vec_a, 0, NULL, NULL);
	CHECK_ERROR(err, "bad return value from clEnqueueWriteBuffer!");

	err = clEnqueueWriteBuffer(commands, d_b, CL_TRUE, 0, sizeof(float) * count,
				   vec_b, 0, NULL, NULL);
	CHECK_ERROR(err, "bad return value from clEnqueueWriteBuffer!");

	err = clEnqueueWriteBuffer(commands, d_c, CL_TRUE, 0, sizeof(float) * count,
				   vec_c, 0, NULL, NULL);
	CHECK_ERROR(err, "bad return value from clEnqueueWriteBuffer!");

	/* set the arguments to our compute kernel */
	err = clSetKernelArg(kernel_vadd, 0, sizeof(cl_mem), &d_a);
	err |= clSetKernelArg(kernel_vadd, 1, sizeof(cl_mem), &d_b);
	err |= clSetKernelArg(kernel_vadd, 2, sizeof(cl_mem), &d_c);
	err |= clSetKernelArg(kernel_vadd, 3, sizeof(unsigned int), &count);
	CHECK_ERROR(err, "bad return value from clSetKernelArg!");

	printf("start executing the program...\n");

	/* start the time for recording the execution time (computation and memory transer) */
	double start_time = time();

	/* execute the kernels over the entire dataset */
	size_t global = count;
	err = clEnqueueNDRangeKernel(commands, kernel_vadd, 1, NULL, &global, NULL, 0,
				     NULL, NULL);
	CHECK_ERROR(err, "bad return value from clEnqueueNDRangeKernel!");

	/* wait until the commands is finished */
	err = clFinish(commands);

	/* read back the result from the computing device */
	err = clEnqueueReadBuffer(commands, d_c, CL_TRUE, 0, sizeof(float) * count, vec_c,
				  0, NULL, NULL);
	if(err != CL_SUCCESS) {
		printf("error, failed to read output array!\n");
		return 0;
	}

	/* stop the timer */
	double end_time = time();

	/* print the time cost */
	printf("finished. it took %lf seconds to run the calculation.\n-\n", end_time - start_time);

	/* test the result */
	float expected_val;
	float deviation;
	for(i = 0; i < count; i++) {
		expected_val = vec_a[i] + vec_b[i];

		//deviation = expected_value - result_from_gpu
		deviation = expected_val - vec_c[i];

		if(fabs(deviation) > 0.001) {
			printf("#%d: the value calculated by gpu is far from"
			       " what we expected!\n", i);
		}
		
	}

	/* print first 10 datas */
	printf("print first 10 datas:\n");
	for(i = 0; i < 10; i++) {
		printf("%f + %f = %f\n", vec_a[i], vec_b[i], vec_c[i]);
	}

	/* release memorys */
	clReleaseMemObject(d_a);
	clReleaseMemObject(d_b);
	clReleaseMemObject(d_c);
	clReleaseProgram(program);
	clReleaseKernel(kernel_vadd);
	clReleaseCommandQueue(commands);
	clReleaseContext(context);

	free(vec_a);
	free(vec_b);
	free(vec_c);

	return 0;
}
