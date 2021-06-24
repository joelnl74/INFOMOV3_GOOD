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
	
	//clEnqueueReadBuffer(queue, matrixBuffer, true, 0, sizeof(EruptionMath::vec3) * 3 * 69451, );

	// Create our buffers.
	//CLhelper::CreateCLBufferFromGLTexture(renderBuffer, context, renderTexture);

	//CLhelper::CreateKernel(kernel, program, "example");
	//CLhelper::SetKernelArg(kernel, 0, sizeof(uint32_t), &width);
	//CLhelper::SetKernelArg(kernel, 1, sizeof(uint32_t), &height);
	//CLhelper::SetKernelArg(kernel, 2, sizeof(int), &offset);
	//CLhelper::SetKernelArg(kernel, 3, sizeof(cl_mem), &renderBuffer);
}

void CLApplication::MatrixMultiplication(int meshSize)
{
	int* test;
	test = (int*)malloc(sizeof(int) * meshSize);

	for (int i = 0; i < meshSize; i++)
		test[i] = 5;

	cl_mem buffer;
	cl_mem hello;
	CLhelper::CreateMemoryBuffer(buffer, context, CL_MEM_WRITE_ONLY, sizeof(int) * meshSize); // 3 is the number of axis
	CLhelper::CreateMemoryBuffer(hello, context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * meshSize, test);

	int i = 0;
	CLhelper::SetKernelArg(kernel, i++, sizeof(cl_mem), &buffer);
	CLhelper::SetKernelArg(kernel, i++, sizeof(int) * meshSize, &hello);

	size_t global = meshSize;
	CLhelper::EnqueueKernel(queue, kernel, 1, &global, 0);
	CLhelper::WaitForQueueToFinish(queue);

	clEnqueueReadBuffer(queue, buffer, true, 0, sizeof(int) * meshSize, test, 0, 0, 0);

	for (int i = 0; i < 10; i++)
		printf("%i\n", test[i]);

	free(test);

	return;
}
