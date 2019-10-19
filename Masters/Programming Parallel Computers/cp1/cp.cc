#include "cp.h"
#include"math.h"
#include"stdlib.h"

inline void RemoveMeans(const float* data , double * ProcessedData , int nx , int ny , double Sum)
{
    for(auto y = 0; y < ny; y++)
    {

        for(auto x = 0; x < nx; x++)
        {
            Sum += data[x + y * nx];
        }

        Sum = Sum / ( nx );

        for(auto x = 0; x < nx; x++)
        {
            ProcessedData[x + y * nx] = data[x + y * nx] - Sum ;
        }

        Sum = 0;
    }

    return;
}


inline void NormalizeSquaredSum(double * ProcessedData , int nx , int ny , double Sum)
{
    for(auto y = 0; y < ny; y++)
    {

        Sum = 0;
        
        for(auto x = 0; x < nx; x++)
        {
            Sum += ( ProcessedData[x + y * nx] * ProcessedData[x + y * nx] );
        }

        for(auto x = 0; x < nx; x++)
        {
            ProcessedData[x + y * nx] = ProcessedData[x + y * nx] / sqrt(Sum);
        }

    }
    
    return;
}

inline void MatrixMultiplication(double * ProcessedData , float* result , int nx , int ny , double Sum)
{
    for(auto i = 0; i < ny; i++)
    {
        for(auto j = i; j < ny; j++)
        {
             Sum = 0;
            
            for(auto k = 0; k < nx; k++)
            {
                Sum += ProcessedData[i * nx + k] * ProcessedData[j * nx + k];
            }

	    result[j + ny*i] = Sum;
        }
    }
    
    return;
}

void correlate(int ny, int nx, const float* data, float* result)
{
    double Sum = 0;

    double * ProcessedData = (double *) calloc(nx * ny , sizeof(double));

    RemoveMeans(data , ProcessedData , nx , ny ,  Sum);
    NormalizeSquaredSum(ProcessedData , nx , ny ,  Sum);
    MatrixMultiplication(ProcessedData , result , nx , ny , Sum);

    free(ProcessedData);
    return;
}
