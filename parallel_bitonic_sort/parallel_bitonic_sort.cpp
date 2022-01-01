/*
 * Written by Nathanael Whitrow
 * a11196694
 * for PDC 2019, assignment 3
*/


#define ITEMS 1024*1024
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Windows.h>


#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)


#include <Windows.h>
double get_wall_time()
{
	LARGE_INTEGER time, freq;
	if (!QueryPerformanceFrequency(&freq))
	{
		//  Handle error
		return 0;
	}
	if (!QueryPerformanceCounter(&time))
	{
		//  Handle error
		return 0;
	}
	return (double)time.QuadPart / freq.QuadPart;
}




int main(void)
{
	// timing vars
	double start_time, end_time, total_time;

	/* create a list of 32bit unsigned ints. */
	int i;
	const int LIST_SIZE = ITEMS;
	cl_uint *INPUT = (cl_uint*)malloc(sizeof(cl_uint)*LIST_SIZE);
	for (i = 0; i < LIST_SIZE; ++i)
		INPUT[i] = rand();

	/*
		Creates a file pointer, string pointer and size var.
		attempts to open kernel.cl
		allocates memory for the source_str
		reads the string and puts it into memory
		closes file
	*/
	
	start_time = get_wall_time();


	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("kernel.cl", "r");
	if (!fp)
	{
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	
	end_time = get_wall_time();
	total_time = end_time - start_time;
	printf("done. List creation time: %f\n", total_time);

	start_time = get_wall_time();

	// OpenCl data structures
	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint err_num_devices;
	cl_uint err_num_platforms;
	cl_int err;

	// Gets the platform ID and places it in platform id. Platform ID is the platform, i.e. nVidia, Intel, etc.
	err = clGetPlatformIDs(1, &platform_id, &err_num_platforms);

	// Given we know the platform is nVidia, get a list of the available GPU's
	err = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &err_num_devices);

	// Makes a context with the GPU in it
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);

	// Sets up a command queue for the context
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, NULL, &err);

	// creates buffers to hold data that can be handed back and forth from GPU memory and host-process memory
	cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, LIST_SIZE * sizeof(cl_uint), NULL, &err);
	//cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, LIST_SIZE * sizeof(cl_uint), NULL, &err);

	// enqueuing a write buffer copies that memory from host process to GPU
	err = clEnqueueWriteBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(cl_uint), INPUT, 0, NULL, NULL);

	// Creates a program  object and loads the strings from kernel.cl into the object
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &err);

	// compiles the program into an executable
	err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	// Creates a kernel object. name must match the kernel name.
	cl_kernel kernel = clCreateKernel(program, "Bitonic_sort", &err);

	// Create arguments to hand to the kernel
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *) &a_mem_obj);

	// Breaks the work up into different sizes and enqueues the kernel
	size_t global_item_size = LIST_SIZE >> 1;
	

	size_t local_item_size;
	err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &local_item_size, NULL);
	printf("local work group size: %d\n", local_item_size);

	cl_event event;

	/* do the thing. */
	cl_uint total_stages = 0;
	cl_uint current_stage, pass, direction_mask, current_item;
	cl_uint direction = 1;
	
	err = clSetKernelArg(kernel, 3, sizeof(cl_uint), (void *)&direction);

	/*
	for (i = 0; i < LIST_SIZE; ++i)
		printf("%d\n", INPUT[i]);
	printf("now do kernel stuff\n");
	*/
	
	
	// figure out how many stages are needed
	for (i = LIST_SIZE; i > 1; i >>= 1)
		total_stages++;
	
	end_time = get_wall_time();
	total_time = end_time - start_time;
	printf("done. setup time: %f\n", total_time);
	
	start_time = get_wall_time();

	for (current_stage = 0; current_stage < total_stages; ++current_stage)
	{
		direction_mask = 1 << current_stage;
		err = clSetKernelArg(kernel, 1, sizeof(cl_uint), (void *) &current_stage);

		for (pass = 0; pass < current_stage + 1; ++pass)
		{
			err = clSetKernelArg(kernel, 2, sizeof(cl_uint), (void *) &pass);

			//printf("stage = %d, pass = %d\n", current_stage, pass);

			err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
		}
	}
	
	end_time = get_wall_time();

	total_time = end_time - start_time;
	printf("done. sorting time: %f\n", total_time);
	/*
	clWaitForEvents(1, &event);
	cl_ulong time_start, time_end;
	double total_time;
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end - time_start;
	*/

	err = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &local_item_size, NULL);
	printf("max item size: %d\n", global_item_size);
	

	// creates space for the output in the host memory and copies the data from GPU memory to host memory
	err = clEnqueueReadBuffer(command_queue, a_mem_obj, CL_TRUE, 0, LIST_SIZE * sizeof(cl_uint), INPUT, 0, NULL, NULL);

	// print out the results
	printf("checking...\n");
	for (i = 0; i < LIST_SIZE - 1; ++i)
		if(INPUT[i] > INPUT[i+1])
			printf("BAD SORT!\n");
	printf("done checking.\n");

	// clean up
	err = clFlush(command_queue);
	err = clFinish(command_queue);
	err = clReleaseKernel(kernel);
	err = clReleaseProgram(program);
	err = clReleaseMemObject(a_mem_obj);
	err = clReleaseCommandQueue(command_queue);
	err = clReleaseContext(context);
	free(INPUT);
	return 0;
}
