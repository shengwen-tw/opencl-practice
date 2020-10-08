#include <stdio.h>
#include <CL/cl.h>

int main(void)
{
	cl_platform_id *platform;
	cl_uint platform_nums;
	cl_int cl_status = clGetPlatformIDs(0, NULL, &platform_nums);

	if(cl_status != CL_SUCCESS) {
		printf("failed to get platform ids\n");
	}

	if(platform_nums == 0) {
		printf("found 0 platforms!\n");	
	}

	cl_device_id *dev_list;
	cl_uint dev_nums;

	return 0;
}
