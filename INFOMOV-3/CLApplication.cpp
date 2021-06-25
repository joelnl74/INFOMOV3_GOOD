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

void CLApplication::MatrixMultiplication(std::vector<EruptionMath::Triangle> triangles, int meshSize)
{
	EruptionMath::Triangle* inputTriangles = (EruptionMath::Triangle*)malloc(sizeof(EruptionMath::Triangle) * meshSize);
	for (int i = 0; i < meshSize; i++) {
		float j = i;
		inputTriangles[i] = triangles[i];
	}

	EruptionMath::Triangle* outputTriangles = (EruptionMath::Triangle*)malloc(sizeof(EruptionMath::Triangle) * meshSize);

	// Input
	buffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(EruptionMath::Triangle) * meshSize, inputTriangles, NULL);
	//output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(EruptionMath::Triangle) * meshSize, NULL, NULL);

	clSetKernelArg(kernel, 0, sizeof(cl_mem), &buffer);
	//clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);

	const size_t global_work_size = meshSize;

	clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_work_size, NULL, 0, NULL, NULL);
	//clEnqueueWriteBuffer(queue, buffer, CL_TRUE, 0, sizeof(EruptionMath::Triangle) * meshSize, outputTriangles, 0, NULL, NULL);
	clEnqueueReadBuffer(queue, buffer, CL_TRUE, 0, sizeof(EruptionMath::Triangle) * meshSize, outputTriangles, 0, NULL, NULL);

	for (int i = 0; i < meshSize; i++)
	{
		EruptionMath::Triangle test = inputTriangles[i];
		printf("%f\n", test.p->x);
	}
}

CLApplication::~CLApplication()
{
	clFlush(queue);
	clFinish(queue);

	clReleaseKernel(kernel); //Release kernel.
	clReleaseProgram(program); //Release the program object.
	clReleaseMemObject(buffer); //Release mem object.
	clReleaseMemObject(output);
	clReleaseCommandQueue(queue); //Release  Command queue.
	clReleaseContext(context); //Release context.
}
