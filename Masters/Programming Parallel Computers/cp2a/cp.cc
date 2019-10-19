#include "cp.h"
#include"math.h"
#include"stdlib.h"

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
        if(nx&1)
        {
              s += ProcessedData[nx - 1 + y * nx] * ProcessedData[nx - 1 + y * nx];
        }
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
            register double s = 0;
            register double t = 0;
            
            for(auto k = 0; k < nx - (nx&1); k+=2)
            {
                s += ProcessedData[i * nx + k] * ProcessedData[j * nx + k];
                t += ProcessedData[i * nx + k+1] * ProcessedData[j * nx + k+1];
            }
	    s += t;
            if(nx&1)
            {
                 s += ProcessedData[i * nx + nx - 1] * ProcessedData[j * nx + nx - 1];    
            }
	    result[j + ny*i] = s;
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
