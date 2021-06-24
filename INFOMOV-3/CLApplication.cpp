#include "CLApplication.h"

void CLApplication::InitCL()
{
	// Select our GPU and create an OpenCL context.
	CLhelper::GetPlatformAndDevice(platformId, deviceId);
	CLhelper::PrintDeviceInfo(deviceId);
	CLhelper::CreateContext(context, platformId, &deviceId);

	// Load and build our OpenCL source code.
	const char* source;
	size_t* sizes = (size_t*)malloc(sizeof(size_t) * 1);
	source = CLhelper::ReadSource("assets/example.cl", sizes);
	CLhelper::CreateProgram(program, context, 1, &source, sizes);
	CLhelper::BuildProgram(program, deviceId);

	// Create required components for our 
	CLhelper::CreateCommandQueue(queue, context, deviceId, NULL);

	CLhelper::CreateKernel(kernel, program, "matrixmul");
}

void CLApplication::MatrixMultiplication(int meshSize)
{
	cl_int input[] = { 0, 1, 2, 3 };
	const size_t global_work_size = sizeof(input);

	buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(input), &input, NULL);

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);

	clFlush(queue);
	clFinish(queue);
	
	clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(input), &input, 0, NULL, NULL);

	printf("%i\n", input[0]);
}
