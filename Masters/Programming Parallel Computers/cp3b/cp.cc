#include "cp.h"
#include <math.h>
#include <iostream>   
#include <stdlib.h>

using namespace std;
typedef float float8_t __attribute__ ((vector_size (8 * sizeof(float))));
constexpr float8_t f8_0{0,0,0,0,0,0,0,0};

static float8_t* float8_alloc(std::size_t n) 
{
    void* tmp = 0;
    if (posix_memalign(&tmp, sizeof(float8_t), sizeof(float8_t) * n)) 
    {
        throw std::bad_alloc();
    }
    return (float8_t*)tmp;
}

inline void CleanTheData(const float* data , float * ProcessedData , unsigned int ny , unsigned int nx)
{
    
    #pragma omp parallel for schedule(static , 1)
    for(unsigned int y = 0; y < ny; y++)
    {
        register float f = 0;
        register float g = 0;
        register unsigned int l = nx - (nx&1);
        
        for(unsigned int x = 0; x < l/*nx - (nx&1)*/; x+=2)
        {
            register float z0 = data[x + y * nx];
            register float z1 = data[x + 1 + y * nx];
            
            f += z0;
            g += z1;
        }
        f += g;
        f += (nx&1) * data[nx - 1 + y * nx];
        f = f / (float)nx;
		//#pragma omp parallel for schedule(static , 1)
        for(unsigned int x = 0; x < nx; x++)
        {
            ProcessedData[x + y * nx] = data[x + y * nx] - f ;
        }

        register float s = 0;
        register float t = 0;
        
        for(unsigned int x = 0; x < l/*nx - (nx&1)*/; x+=2)
        {
            s += ( ProcessedData[x + y * nx] * ProcessedData[x + y * nx] );
	    	t += ( ProcessedData[x + 1 + y * nx] * ProcessedData[x + 1 + y * nx] );
        }

		s += t;
        s += (nx&1) * ProcessedData[nx - 1 + y * nx] * ProcessedData[nx - 1 + y * nx];
        register float w = sqrt(s);
        //#pragma omp parallel for// schedule(static , 1)
        for(unsigned int x = 0; x < nx; x++)
        {	
			float h = ProcessedData[x + y * nx] / w;
            ProcessedData[x + y * nx] = h;
        }
    }

    return;
}

void correlate( int ny, int nx, const float* data, float* result ) 
{
    constexpr int nb = 8;           
    int nxa = ( nx + nb - 1 ) / nb; 
    constexpr int nd = 8;       
    int nc = (ny + nd - 1) / nd;   
    int nya = nc * nd;

    float * ProcessedData = (float *)calloc( nx * ny , sizeof(float));
    float8_t * vd = float8_alloc(nya * nxa);
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
            vd[cc + x] = f8_0;
        }
    }

    #pragma omp parallel for schedule(dynamic,1)
    for( int ic = 0; ic < nc; ic++ )
    {
        for( int jc = ic; jc < nc; jc++ )
        {
            float8_t partials[nd][nd];
            
            for( int id = 0; id < nd; id++ )
            {
                for( int jd = 0; jd < nd; jd++ )
                {
                    partials[id][jd] = f8_0;
                }
            }

            for( int ka = 0; ka < nxa; ka++ )
            {

                float8_t x0 = vd[ nxa * (ic * nd + 0) + ka ];
                float8_t x1 = vd[ nxa * (ic * nd + 1) + ka ];
                float8_t x2 = vd[ nxa * (ic * nd + 2) + ka ];
                float8_t x3 = vd[ nxa * (ic * nd + 3) + ka ];
                float8_t x4 = vd[ nxa * (ic * nd + 4) + ka ];
                float8_t x5 = vd[ nxa * (ic * nd + 5) + ka ];
                float8_t x6 = vd[ nxa * (ic * nd + 6) + ka ];
                float8_t x7 = vd[ nxa * (ic * nd + 7) + ka ];

                float8_t y0 = vd[ nxa * (jc * nd + 0) + ka ];
                float8_t y1 = vd[ nxa * (jc * nd + 1) + ka ];
                float8_t y2 = vd[ nxa * (jc * nd + 2) + ka ];
                float8_t y3 = vd[ nxa * (jc * nd + 3) + ka ];
                float8_t y4 = vd[ nxa * (jc * nd + 4) + ka ];
                float8_t y5 = vd[ nxa * (jc * nd + 5) + ka ];
                float8_t y6 = vd[ nxa * (jc * nd + 6) + ka ];
                float8_t y7 = vd[ nxa * (jc * nd + 7) + ka ];

                partials[0][0] += x0 * y0;
                partials[0][1] += x0 * y1;
                partials[0][2] += x0 * y2;
                partials[0][3] += x0 * y3;
                partials[0][4] += x0 * y4;
                partials[0][5] += x0 * y5;
                partials[0][6] += x0 * y6;
                partials[0][7] += x0 * y7;

                partials[1][0] += x1 * y0;
                partials[1][1] += x1 * y1;
                partials[1][2] += x1 * y2;
                partials[1][3] += x1 * y3;
                partials[1][4] += x1 * y4;
                partials[1][5] += x1 * y5;
                partials[1][6] += x1 * y6;
                partials[1][7] += x1 * y7;

                partials[2][0] += x2 * y0;
                partials[2][1] += x2 * y1;
                partials[2][2] += x2 * y2;   
                partials[2][3] += x2 * y3;
                partials[2][4] += x2 * y4;
                partials[2][5] += x2 * y5;
                partials[2][6] += x2 * y6;
                partials[2][7] += x2 * y7; 

                partials[3][0] += x3 * y0;
                partials[3][1] += x3 * y1;
                partials[3][2] += x3 * y2;
                partials[3][3] += x3 * y3;
                partials[3][4] += x3 * y4;
                partials[3][5] += x3 * y5;
                partials[3][6] += x3 * y6;
                partials[3][7] += x3 * y7;

                partials[4][0] += x4 * y0;
                partials[4][1] += x4 * y1;
                partials[4][2] += x4 * y2;
                partials[4][3] += x4 * y3;
                partials[4][4] += x4 * y4;
                partials[4][5] += x4 * y5;
                partials[4][6] += x4 * y6;
                partials[4][7] += x4 * y7;

                partials[5][0] += x5 * y0;
                partials[5][1] += x5 * y1;
                partials[5][2] += x5 * y2;
                partials[5][3] += x5 * y3;
                partials[5][4] += x5 * y4;
                partials[5][5] += x5 * y5;
                partials[5][6] += x5 * y6;
                partials[5][7] += x5 * y7;

                partials[6][0] += x6 * y0;
                partials[6][1] += x6 * y1;
                partials[6][2] += x6 * y2;
                partials[6][3] += x6 * y3;
                partials[6][4] += x6 * y4;
                partials[6][5] += x6 * y5;
                partials[6][6] += x6 * y6;
                partials[6][7] += x6 * y7;

                partials[7][0] += x7 * y0;
                partials[7][1] += x7 * y1;
                partials[7][2] += x7 * y2;
                partials[7][3] += x7 * y3;
                partials[7][4] += x7 * y4;
                partials[7][5] += x7 * y5;
                partials[7][6] += x7 * y6;
                partials[7][7] += x7 * y7;

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
                        partials[id][jd][0] += partials[id][jd][4];
                        partials[id][jd][0] += partials[id][jd][5];
                        partials[id][jd][0] += partials[id][jd][6];
                        partials[id][jd][0] += partials[id][jd][7];
                        result[ ny * i + j ] = partials[id][jd][0];
                    }
                }
            }
        }
    }
    std::free(vd);
    std::free(ProcessedData);
}
