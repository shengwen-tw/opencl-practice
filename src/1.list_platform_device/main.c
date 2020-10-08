#include <stdio.h>
#include <CL/cl.h>

int main(void)
{
	cl_uint platform_nums;
	cl_int cl_status = clGetPlatformIDs(0, NULL, &platform_nums);

	if(cl_status != CL_SUCCESS) {
		printf("error: failed to get platform ids\n");
		return 0;
	}

	if(platform_nums == 0) {
		printf("error: found 0 platforms!\n");
		return 0;
	}

	printf("number of opencl platform: %d\n", platform_nums);

	cl_platform_id platforms[platform_nums];
	cl_status = clGetPlatformIDs(platform_nums, platforms, NULL);

	cl_char s[10000] = {0};

	int i, j, k;
	for(i = 0; i < platform_nums; i++) {
		printf("-\nplatform number %d:\n", i + 1);

		/* platform informations */
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(s), &s, NULL);
		printf("|- platform name: %s\n", s);

		clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(s), &s, NULL);
		printf("|- platform vendor: %s\n", s);

		clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(s), &s, NULL);
		printf("|- platform opencl version: %s\n", s);

		/* device informations of current platform */
		cl_uint dev_nums;
		cl_status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &dev_nums);
		printf("|- number of devices: %d\n", dev_nums);

		cl_device_id devs[dev_nums];
		cl_status = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, dev_nums, devs, &dev_nums);

		for(j = 0; j < dev_nums; j++) {
			printf(" |- device number %d:\n", j + 1);

			clGetDeviceInfo(devs[j], CL_DEVICE_NAME, sizeof(s), &s, NULL);
			printf(" |- device name: %s\n", s);

			clGetDeviceInfo(devs[j], CL_DEVICE_OPENCL_C_VERSION, sizeof(s), &s, NULL);
			printf(" |- device opencl c version: %s\n", s);

			cl_uint max_compute_units;
			clGetDeviceInfo(devs[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint),
					&max_compute_units, NULL);
			printf(" |- device max compute units: %d\n", max_compute_units);

			cl_long mem_size;
			clGetDeviceInfo(devs[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_long),
					&mem_size, NULL);
			printf(" |- device local memory size: %luKB\n", mem_size / 1024);

			clGetDeviceInfo(devs[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_long),
					&mem_size, NULL);
			printf(" |- device global memory size: %luMB\n", mem_size / (1024 * 1024));
			
			clGetDeviceInfo(devs[j], CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_long),
					&mem_size, NULL);
			printf(" |- device max allocation size: %luMB\n", mem_size / (1024 * 1024));

			size_t max_workgroup_size;
			clGetDeviceInfo(devs[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t),
					&max_workgroup_size, NULL);
			printf(" |- device max workgroup size: %ld\n", max_workgroup_size);

			size_t max_dims;
			clGetDeviceInfo(devs[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(size_t),
					&max_dims, NULL);
			printf(" |- device max work item dimensions: %ld\n", max_dims);

			size_t dims[max_dims];
			clGetDeviceInfo(devs[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(dims),
					&dims, NULL);

			for(k = 0; k < max_dims; k++) {
				printf("  |- device work item size of number %d: %ld\n", k + 1, dims[k]);
			}
		}
	}

	return 0;
}
