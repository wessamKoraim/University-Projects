#include "cp.h"
#include <math.h>
#include <iostream>   
#include <stdlib.h>

using namespace std;
typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));
constexpr double4_t d4_0{0,0,0,0};

static double4_t* double4_alloc(std::size_t n) 
{
    void* tmp = 0;
    if (posix_memalign(&tmp, sizeof(double4_t), sizeof(double4_t) * n)) 
    {
        throw std::bad_alloc();
    }
    return (double4_t*)tmp;
}

inline void CleanTheData(const float* data , double * ProcessedData , unsigned int ny , unsigned int nx)
{
    
    #pragma omp parallel for schedule(static , 1)
    for(unsigned int y = 0; y < ny; y++)
    {
        register double f = 0;
        register double g = 0;
        register unsigned int l = nx - (nx&1);
        
        for(unsigned int x = 0; x < l/*nx - (nx&1)*/; x+=2)
        {
            register double z0 = data[x + y * nx];
            register double z1 = data[x + 1 + y * nx];
            
            f += z0;
            g += z1;
        }
        f += g;
        f += (nx&1) * data[nx - 1 + y * nx];
        f = f / (double)nx;
		//#pragma omp parallel for schedule(static , 1)
        for(unsigned int x = 0; x < nx; x++)
        {
            ProcessedData[x + y * nx] = data[x + y * nx] - f ;
        }

        register double s = 0;
        register double t = 0;
        
        for(unsigned int x = 0; x < l/*nx - (nx&1)*/; x+=2)
        {
            s += ( ProcessedData[x + y * nx] * ProcessedData[x + y * nx] );
	    	t += ( ProcessedData[x + 1 + y * nx] * ProcessedData[x + 1 + y * nx] );
        }

		s += t;
        s += (nx&1) * ProcessedData[nx - 1 + y * nx] * ProcessedData[nx - 1 + y * nx];
        register double w = sqrt(s);
        //#pragma omp parallel for// schedule(static , 1)
        for(unsigned int x = 0; x < nx; x++)
        {	
			double h = ProcessedData[x + y * nx] / w;
            ProcessedData[x + y * nx] = h;
        }
    }

    return;
}

void correlate( int ny, int nx, const float* data, float* result ) 
{
    constexpr int nb = 4;           
    int nxa = ( nx + nb - 1 ) / nb; 
    constexpr int nd = 4;       
    int nc = (ny + nd -1)/nd;   
    int nya = nc * nd;          

    double * ProcessedData = (double *)calloc(nx*ny,sizeof(double));
    double4_t *vd = double4_alloc(nya * nxa);
    CleanTheData(data , ProcessedData , ny , nx);

    #pragma omp parallel for schedule(dynamic,1)
    for( int y = 0; y < ny; y++ )
    {
        for( int x = 0; x < nxa; x++ )
        {
            for( int kb = 0; kb < nb; kb++ )
            {
                int i = x * nb + kb;
                int cc = nx * y;
                int zz = nxa * y;
				vd[ zz + x ][ kb ] = (i < nx) ? ProcessedData[ cc + i ] : 0;
            }
        }
    }

    
    for( int y = ny; y < nya; y++ )
    {
        for( int x = 0; x < nxa; x++ )
        {
            int cc = nxa * y;
            vd[cc + x] = d4_0;
        }
    }

    #pragma omp parallel for schedule(dynamic,1)
    for( int ic = 0; ic < nc; ++ic )
    {
        for( int jc = ic; jc < nc; jc++ )
        {
            double4_t partials[nd][nd];
            
            for( int id = 0; id < nd; ++id )
            {
                for( int jd = 0; jd < nd; jd++ )
                {
                    partials[id][jd] = d4_0;
                }
            }
            for( int ka = 0; ka < nxa; ++ ka )
            {
                int cc = ic * nd;
                double4_t x0 = vd[ nxa * (cc + 0) + ka ];
                double4_t x1 = vd[ nxa * (cc + 1) + ka ];
                double4_t x2 = vd[ nxa * (cc + 2) + ka ];
                double4_t x3 = vd[ nxa * (cc + 3) + ka ];
                int zz = jc * nd;
                double4_t y0 = vd[ nxa * (zz + 0) + ka ];
                double4_t y1 = vd[ nxa * (zz + 1) + ka ];
                double4_t y2 = vd[ nxa * (zz + 2) + ka ];
                double4_t y3 = vd[ nxa * (zz + 3) + ka ];

                partials[0][0] += x0 * y0;
                partials[0][1] += x0 * y1;
                partials[0][2] += x0 * y2;
                partials[0][3] += x0 * y3;

                partials[1][0] += x1 * y0;
                partials[1][1] += x1 * y1;
                partials[1][2] += x1 * y2;
                partials[1][3] += x1 * y3;

                partials[2][0] += x2 * y0;
                partials[2][1] += x2 * y1;
                partials[2][2] += x2 * y2;   
                partials[2][3] += x2 * y3; 

                partials[3][0] += x3 * y0;
                partials[3][1] += x3 * y1;
                partials[3][2] += x3 * y2;   
                partials[3][3] += x3 * y3; 
            }
            for( int id = 0; id < nd; id++ )
            {
                for( int jd = 0; jd < nd; jd++ )
                {
                    int i = ic * nd + id;
                    int j = jc * nd + jd;
                    if( i < ny && j < ny )
                    {
                        partials[id][jd][0] += partials[id][jd][1];
                        partials[id][jd][0] += partials[id][jd][2];
                        partials[id][jd][0] += partials[id][jd][3];
                        result[ ny * i + j ] = partials[id][jd][0];
                    }
                }
            }
        }
    }
    free(vd);
    free(ProcessedData);
}



