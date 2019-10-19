#include "cp.h"
#include <cuda_runtime.h>
#include <cstdlib>
#include <iostream>
#include<vector>
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

inline int static roundup(int a, int b) 
{
    return divup(a, b) * b;
}

__global__ void mykernel(int nx, int ny, const float * data, float * result)
{
    int ia = threadIdx.x;
    int ja = threadIdx.y;
    int ic = blockIdx.x;
    int jc = blockIdx.y;

    if( ic < jc )
    {
        return;
    }
    float v[ 8 ][ 8 ];
    for( int ib = 0; ib < 8; ++ib )
    {
        for( int jb = 0; jb < 8; ++jb )
        {
            v[ ib ][ jb ] = 0;
        }
    }
    for( int k = 0; k < nx; ++k )
    {
        float x[ 8 ];
        float y[ 8 ];
        for( int ib = 0; ib < 8; ++ ib )
        {
            int i = ic * 64 + ib * 8 + ia;            
            x[ ib ] = data[ ny * k + i ];
            for( int jb = 0; jb < 8; ++ jb )
            {
                int j = jc * 64 + jb * 8 + ja;
                y[ jb ] = data[ ny * k + j ];
            }
            
        }       
        for( int ib = 0; ib < 8; ++ ib )
        {
            for( int jb = 0; jb < 8; ++jb )
            {
                v[ ib ][ jb ] += x[ ib ] * y[ jb ];
            }
        }
    }
    for( int ib = 0; ib < 8; ++ib )
    {
        for( int jb = 0; jb < 8; ++jb )
        {
            int i = ic * 64 + ib * 8 + ia;
            int j = jc * 64 + jb * 8 + ja;
            if( i < ny && j < ny )
            {
                result[ ny * j + i ] = v[ ib ][ jb ];
            }
        }
    }
}

__global__ void myppkernel( int nx, int ny, const float * data, float * data_copy )
{
    int y = threadIdx.x + blockIdx.x * blockDim.x ;    
    if( y >= ny )
    {
        return;
    }
    float s = 0.0;
    float t = 0.0;
    for( int x = 0; x < nx; ++x )
    {
        s += data[ x + y * nx ];
    }
    s = s / (float)nx;
    for( int x = 0; x < nx; ++x )
    {
        data_copy[ y + x * ny ] = data[ x + y*nx ] -s;
        t +=  data_copy[ y + x * ny ]* data_copy[ y + x * ny ];
    }
    t = sqrt( t );
    for( int x = 0; x < nx; x ++ )
    {
        data_copy[ y + x * ny ] = data_copy[ y + x * ny ]/t;
    }
}

void correlate(int ny, int nx, const float* data, float* result) 
{
    //preprocess data into data_copy
    int ny_64 = roundup( ny, 64 );

    //Allocate memory and copy data to GPU
    float * dGPU = NULL;//data for GPU
    CHECK(cudaMalloc((void**)&dGPU, ny_64 * ny_64 * sizeof(float)));
    
    float *dpGPU = NULL;//original data for GPU preproc
    CHECK(cudaMalloc((void**)&dpGPU, nx * ny * sizeof(float)));

    float * rGPU = NULL;//result for GPU
    CHECK(cudaMalloc((void**)&rGPU, ny * ny * sizeof(float)));
    
    CHECK(cudaMemcpy(dpGPU, data, nx * ny * sizeof(float), cudaMemcpyHostToDevice));

    //Run kernel
    {
        dim3 dimBlock(64);
        dim3 dimGrid( divup(ny,64) );
        myppkernel<<<dimGrid, dimBlock>>>( nx, ny, dpGPU, dGPU );
    }
    //Run kernel
    {
        dim3 dimBlock(8, 8);
        dim3 dimGrid( ny_64/64, ny_64/64 );
        mykernel<<<dimGrid, dimBlock>>>( nx, ny, dGPU, rGPU);
        CHECK(cudaGetLastError());
    }
    // Copy data back to CPU & release memory
    CHECK(cudaMemcpy(result, rGPU, ny * ny * sizeof(float), cudaMemcpyDeviceToHost ));
    CHECK(cudaFree(dGPU));
    CHECK(cudaFree(rGPU));
}
