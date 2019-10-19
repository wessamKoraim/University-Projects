#include "cp.h"
#include"math.h"
#include"stdlib.h"

typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));

constexpr double4_t d4_0 = {0,0,0,0};

inline void RemoveMeans(const float* data , double * ProcessedData , int nx , int ny)
{

    for(auto y = 0; y < ny; y++)
    {
        register double s = 0;
        for(auto x = 0; x < nx; x++)
        {
            s += data[x + y * nx];
        }

        s = s / ( nx );

        for(auto x = 0; x < nx; x++)
        {
            ProcessedData[x + y * nx] = data[x + y * nx] - s ;
        }
    }

    return;
}


inline void NormalizeSquaredSum(double * ProcessedData , int nx , int ny)
{
    for(auto y = 0; y < ny; y++)
    {

        register double s = 0;
        register double t = 0;
        
        for(auto x = 0; x < nx - (nx&1); x+=2)
        {
            s += ( ProcessedData[x + y * nx] * ProcessedData[x + y * nx] );
	        t += ( ProcessedData[x + 1 + y * nx] * ProcessedData[x + 1 + y * nx] );
        }

        s += t;

        s += (nx&1) * ProcessedData[nx - 1 + y * nx] * ProcessedData[nx - 1 + y * nx];
        
        for(auto x = 0; x < nx; x++)
        {
            ProcessedData[x + y * nx] = ProcessedData[x + y * nx] / sqrt(s);
        }

    }
    
    return;
}

inline void MatrixMultiplication(double * ProcessedData , float* result , int nx , int ny)
{
    for(auto i = 0; i < ny; i++)
    {
        for(auto j = i; j < ny; j++)
        {
            register double4_t r = d4_0;
            for(auto k = 0; k < nx/4; k ++)
            {
            	register double4_t s;
            	for(int l = 0; l < 4; l ++)
            	{
            		s[l] = ProcessedData[i * nx + k*4+l] * ProcessedData[j * nx + k*4+l];
            	}
            	r += s;
            }
            register double sum = 0;
            for(auto l = 0; l < nx%4; l ++ )
            {
            	sum += ProcessedData[i * nx + nx - l -1 ] * ProcessedData[j * nx + nx - l - 1];
            }
            
            for(int i = 0; i < 4; i ++)
            {
            	sum += r[i];
            }
	    result[j + ny*i] = sum;
        }
    }
    
    return;
}

void correlate(int ny, int nx, const float* data, float* result)
{

    double * ProcessedData = (double *) calloc(nx * ny , sizeof(double));

    RemoveMeans(data , ProcessedData , nx , ny);
    NormalizeSquaredSum(ProcessedData , nx , ny);
    MatrixMultiplication(ProcessedData , result , nx , ny);

    free(ProcessedData);
    return;
}

