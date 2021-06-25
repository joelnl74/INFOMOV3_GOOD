typedef struct _Triangle
{
    float3 p1;
    float3 p2;
    float3 p3;
}Triangle;

__kernel void matrixmul (__global Triangle* input, __global Triangle* output) {
  int id = get_global_id(0);
  output[id].p1.x = 10.0f;
  // input[id].p1.x = 10.0f;
  // output[id] = input[id];
}