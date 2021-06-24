#pragma once
#include "cl_helper.h"
#include "BasicShader.h"
#include <vector>
#include "BasicShader.h"

class CLApplication
{
public:
	/** Initializes everything for OpenCL. */
	void InitCL();

	void MatrixMultiplication(int meshSize);

private:
	/* ==== EXAMPLE DATA ==== */

/** Our platform. */
	cl_platform_id platformId;
	/** GPU handle. */
	cl_device_id deviceId;
	/** OpenCL context. */
	cl_context context;
	/** Our main command queue. */
	cl_command_queue queue;
	/** OpenCL program. */
	cl_program program;
	/** OpenCL template. */
	cl_kernel kernel;
	/** OpenGL render target. */
	cl_mem renderBuffer;
};

