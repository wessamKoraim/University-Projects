#include "is.h"
#include <cuda_runtime.h>
#include <iostream>

using namespace std;

inline void check(cudaError_t err, const char* context) 
{
    if (err != cudaSuccess) 
    {
        std::cerr << "CUDA error: " << context << ": "
            << cudaGetErrorString(err) << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
#define CHECK(x) check(x, #x)
inline int static divup(int a, int b) 
{
    return (a + b - 1)/b;
}

__global__ void mykernel(int nx, int ny, const float * data, float * result)
{
    int len_x = threadIdx.x + blockIdx.x * blockDim.x;
    int len_y = threadIdx.y + blockIdx.y * blockDim.y;

    if( len_x == 0 || len_y ==0 || len_x > nx || len_y > ny || ( len_x == nx && len_y == ny ) )
    {
        return;
    }
    const int sum_nx = nx + 1;
    const int sum_ny = ny + 1;
    const int X = len_x * len_y;
    const int Y = nx * ny - X;
    const float X_inv = 1/(float)X;
    const float Y_inv = 1/(float)Y;
    const float sy = data[( sum_nx - 1 )  + sum_nx * ( sum_ny -1 )];
    float h_max = 0;
    for( int pos_y = 1; pos_y <= sum_ny - len_y; ++pos_y )
    {
        for( int pos_x = 1; pos_x <= sum_nx - len_x; pos_x++ )
        {

            int y1 = pos_y + len_y - 1;
            int x1 = pos_x + len_x - 1;
            float s1 = data[x1 + sum_nx * y1];
            float s2 = data[x1 + sum_nx * ( pos_y - 1 )];
            float s3 = data[( pos_x - 1 ) + sum_nx * y1];
            float s4 = data[ ( pos_x - 1 ) + sum_nx * ( pos_y - 1) ];
            float vx = s1 - s2 - s3 + s4;
            float vy = sy - vx;
            float a = (vx * vx) * X_inv + vy * vy * Y_inv;
            if( a > h_max )
            {
                h_max = a;
            }
        }
    }
    result[ len_x + len_y * sum_nx ] = h_max;
}

Result segment(int ny, int nx, const float* data) 
{
    Result result { 0, 0, 0, 0, {0, 0, 0}, {0, 0, 0} };
    
    const int sum_nx = nx + 1;
    const int sum_ny = ny + 1;
    float * s = ( float * )calloc( sum_nx * sum_ny , sizeof(float)  ) ;

    for(int x = 0; x < sum_nx; x++ )
    {
        s[x] = 0;
    }
    for(int y = 0; y < sum_ny; y++ )
    {
        s[sum_nx * y] = 0;
    }

    s[ 1 + sum_nx] = data[0];


    for( int x = 3; x < 3 * nx; x += 3 )
    {
        s[ 1 + x/3 + sum_nx ] = data[ x ] + s[ 1 + x/3 - 1 + sum_nx ];
    }
    for ( int y = 2; y < sum_ny; ++y )
    {
        for ( int x = 1; x < sum_nx; ++x )
        {
            int ind  =  x + sum_nx * y;         
            int ind3 =  x + sum_nx * ( y - 1 ); 
            int ind2 = ( x -1 ) + sum_nx * y;   
            int ind4 = ( x -1 ) + sum_nx * (y-1);
            s[ ind ] = data[ 3 * (x-1) + 3 * nx * (y-1)  ] + s[ ind2 ] - s[ ind4 ] + s[ ind3 ];
            
        }
    }

    float * dGPU = NULL;
    CHECK(cudaMalloc((void**)&dGPU, sum_nx * sum_ny * sizeof(float)));
    CHECK(cudaMemcpy(dGPU, s, sum_nx * sum_ny * sizeof(float), cudaMemcpyHostToDevice));

    float * rGPU = NULL;
    CHECK(cudaMalloc((void**)&rGPU, sum_nx * sum_ny * sizeof(float)));

    
    {
        dim3 dimBlock(8, 8);
        dim3 dimGrid( divup(nx,dimBlock.x), divup(ny,dimBlock.y) );
        mykernel<<<dimGrid, dimBlock>>>( nx, ny, dGPU, rGPU);
        CHECK(cudaGetLastError());
    }
    float * result_array = (float * )malloc( sizeof(float) * sum_nx * sum_ny ) ; 
    CHECK(cudaMemcpy(result_array, rGPU, sum_nx * sum_ny * sizeof(float), cudaMemcpyDeviceToHost ));

    float h_max = 0;
    int best_X = 0, best_Y = 0;
    for( int len_y = 1; len_y < sum_ny; ++len_y )
    {
        for( int len_x = 1; len_x < sum_nx; ++len_x )
        {
            if( result_array[ len_x + sum_nx * len_y ] > h_max )
            {
                h_max = result_array[ len_x + sum_nx * len_y ];
                best_X = len_x;
                best_Y = len_y;
            }
        }
    }


    const int X = best_X  * best_Y;
    const int Y = nx * ny - X;
    const float X_inv = 1/(float)X;
    const float Y_inv = 1/(float)Y;
    const float sy = s[ ( sum_nx - 1 )  + sum_nx * ( sum_ny - 1 )];

    h_max = 0;
    for( int pos_y = 1; pos_y <= sum_ny - best_Y; pos_y++ )
    {
        for( int pos_x = 1; pos_x <= sum_nx - best_X; pos_x++ )
        {

            int y1 = pos_y + best_Y - 1;
            int x1 = pos_x + best_X - 1;
            float s1 = s[ x1 + sum_nx * y1 ];
            float s2 = s[ x1 + sum_nx * ( pos_y - 1 ) ];
            float s3 = s[ ( pos_x - 1 ) + sum_nx * y1 ];
            float s4 = s[ ( pos_x - 1 ) + sum_nx * ( pos_y - 1) ];
            float vx = s1 - s2 - s3 + s4;
            float vy = sy - vx;
            float a = (vx * vx) * X_inv + (vy * vy) * Y_inv;

            if( a > h_max )
            {
                vx *= X_inv;
                vy *= Y_inv;
                result.x0 = pos_x-1;
                result.x1 = x1;
                result.y0 = pos_y-1;
                result.y1 = y1;
                result.outer[ 0 ] = (vy);
                result.outer[ 1 ] = (vy);
                result.outer[ 2 ] = (vy);
                result.inner[ 0 ] = (vx);
                result.inner[ 1 ] = (vx);
                result.inner[ 2 ] = (vx);
                h_max = a;
            }
        }
    }

    CHECK(cudaFree(dGPU));
    CHECK(cudaFree(rGPU));
    std::free(s);
	std::free(result_array);
 	return result;
}
