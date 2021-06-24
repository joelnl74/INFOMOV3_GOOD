__kernel void matrixmul (__global int* output) {
	output[get_global_id(0)]++;
}