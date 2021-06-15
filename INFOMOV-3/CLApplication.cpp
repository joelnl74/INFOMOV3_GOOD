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

	// Create our buffers.
	//CLhelper::CreateCLBufferFromGLTexture(renderBuffer, context, renderTexture);

	//CLhelper::CreateKernel(kernel, program, "example");
	//CLhelper::SetKernelArg(kernel, 0, sizeof(uint32_t), &width);
	//CLhelper::SetKernelArg(kernel, 1, sizeof(uint32_t), &height);
	//CLhelper::SetKernelArg(kernel, 2, sizeof(int), &offset);
	//CLhelper::SetKernelArg(kernel, 3, sizeof(cl_mem), &renderBuffer);
}
