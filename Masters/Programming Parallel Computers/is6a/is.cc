#include "is.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <omp.h>
#include <x86intrin.h>
#include <immintrin.h>

typedef float float8_t __attribute__ ((vector_size (8* sizeof(float))));
const float8_t f8_0 = {0 , 0 , 0 , 0 , 0 , 0 , 0 , 0};

inline float8_t max8(float8_t x, float8_t y) 
{
    return x > y ? x : y;
}
inline float h_max8( float8_t x )
{
    float max = 0;
    for( int i = 0; i < 8; i++ )
    {
        if( x[i] > max )
        {
            max = x[ i ];
        }
    }
    return max;
}

Result segment(int ny, int nx, const float* data) 
{
    Result result { 0, 0, 0, 0, {0, 0, 0}, {0, 0, 0} };

    const int sum_nx = nx + 1;
    const int sum_ny = ny + 1;

    float * s = (float * ) calloc( sum_nx * sum_ny , sizeof(float) );

    s[1 + sum_nx] = data[0];

    for(int y = 0; y < sum_ny; y++ )
    {
        s[sum_nx * y] = 0;
    }

    for(int x = 0; x < sum_nx; x++ )
    {
        s[x] = 0;
    }

    for( int x = 3; x < (3 * nx); x += 3 )
    {
        s[1 + x/3 + sum_nx] = data[x] + s[1 + x/3 - 1 + sum_nx];
    }

    for (int y = 2; y < sum_ny; y++)
    {
        for (int x = 1; x < sum_nx; x++)
        {
            int ind  =  x + sum_nx * y;        
            int ind_3 =  x + sum_nx * (y - 1);
            int ind_2 = ( x - 1 ) + sum_nx * y;    
            int ind_4 = ( x - 1 ) + sum_nx * (y - 1);
            s[ind] = data[3 * (x-1) + 3 * nx * (y-1)] + s[ind_2] - s[ind_4] + s[ind_3];
        }
    }
 
    const float s_last = s[( sum_nx - 1 )  + sum_nx * ( sum_ny - 1 )];

    int best_X = 0, best_Y = 0;
    float h_max = 0;

    #pragma omp parallel for schedule (dynamic,1)
    for( int len_y = 1; len_y <= ny; len_y++ )
    {
        for( int len_x = 1; len_x <= nx; len_x++ )
        {
        	if( (len_x == nx) && (len_y == ny) )
        	{
        		continue;
        	}
            
            const int X = len_x  * len_y;
            const int Y = nx * ny - X;
            const float X_inv = 1 / (float)X;
            const float Y_inv = 1 / (float)Y;
            const float const_0 = X_inv + Y_inv;
            const float const_1 = s_last * s_last * Y_inv;
            const float const_2 = 2 * s_last * Y_inv;

            float8_t max_utility = f8_0;
            float max_0 = 0;

            for( int pos_y = 1; pos_y <= sum_ny - len_y; pos_y++ )
            {
                int y1 = pos_y + len_y - 1;
                
                auto tmp_var_0 = std::min(7, sum_nx - len_x);
                for( int pos_x = 1; pos_x <= tmp_var_0; pos_x++ )
                {
                    int x1 = pos_x + len_x - 1;
                    float s1 = s[ x1 + sum_nx * y1 ];
                    float s2 = s[ x1 + sum_nx * ( pos_y - 1 ) ];
                    float s3 = s[ ( pos_x - 1 ) + sum_nx * y1 ];
                    float s4 = s[ ( pos_x - 1 ) + sum_nx * ( pos_y - 1) ];
                    float vx = s1 - s2 - s3 + s4;
                    float max_1 = vx * vx * const_0 + const_1 - const_2 * vx;

                    if( max_1 > max_0 )
                    {
                        max_0 = max_1;
                    }
                }

                int f8_sum_nx = ( sum_nx - len_x ) / 8;

                for( int pos_x = 1; pos_x < f8_sum_nx; pos_x++ )
                {
                    const int x1 = pos_x * 8 + len_x - 1 ;

                    float8_t s1 = _mm256_loadu_ps( (s + ( x1 + sum_nx * y1 ) ) );
                    float8_t s2 = _mm256_loadu_ps( (s + ( x1 + sum_nx * ( pos_y - 1 ) ) ) );
                    float8_t s3 = _mm256_loadu_ps( (s + ( ( pos_x * 8 - 1 ) + sum_nx * y1 ) ) );
                    float8_t s4 = _mm256_loadu_ps( (s + ( ( pos_x * 8 - 1 ) + sum_nx * ( pos_y - 1 ) ) ) );
                    float8_t vx = s1 - s2 - s3 + s4;

                    max_utility = max8(max_utility, vx * vx * const_0 + const_1 - const_2 * vx);
                }

                auto tmp_var_1 = std::max( 1, f8_sum_nx * 8 );
                for( int pos_x = tmp_var_1; pos_x <= sum_nx - len_x; pos_x++ )
                {
                    const int x1 = pos_x + len_x - 1;

                    const float s1 = s[x1 + sum_nx * y1];
                    const float s2 = s[x1 + sum_nx * ( pos_y - 1 )];
                    const float s3 = s[( pos_x - 1 ) + sum_nx * y1];
                    const float s4 = s[( pos_x - 1 ) + sum_nx * ( pos_y - 1)];
                    const float vx = s1 - s2 - s3 + s4;

                    const float max_1 = vx * vx * const_0 + const_1 - const_2 * vx;

                    if( max_1 > max_0 )
                    {
                        max_0 = max_1;
                    }
                }

            }

            float max_2 = h_max8(max_utility);
            if( max_0 > max_2 )
            {
                max_2 = max_0;
            }

            #pragma omp critical
            {
                if( max_2 > h_max ) 
                {   
                    h_max = max_2;
                    best_X = len_x;
                    best_Y = len_y;
                }
            }
        }
    }

    const int X = best_X  * best_Y;
    const int Y = nx * ny - X;
    const float X_inv = 1/(float)X;
    const float Y_inv = 1/(float)Y;

    h_max = 0;

    for( int pos_y = 1; pos_y <= sum_ny - best_Y; pos_y++ )
    {
        for( int pos_x = 1; pos_x <= sum_nx - best_X; pos_x++ )
        {

            int y1 = pos_y + best_Y - 1;
            int x1 = pos_x + best_X - 1;
            float s1 = s[x1 + sum_nx * y1];
            float s2 = s[x1 + sum_nx * ( pos_y - 1 )];
            float s3 = s[( pos_x - 1 ) + sum_nx * y1];
            float s4 = s[( pos_x - 1 ) + sum_nx * ( pos_y - 1)];
            float vx = s1 - s2 - s3 + s4;
            float vy = s_last - vx;
            float a = (vx * vx) * X_inv + vy * vy * Y_inv;
            if( a > h_max )
            {
                vx *= X_inv;
                vy *= Y_inv;
                result.x0 = pos_x - 1;
                result.x1 = x1;
                result.y0 = pos_y - 1;
                result.y1 = y1;
                result.outer[1] = (vy);
                result.outer[2] = (vy);
                result.inner[0] = (vx);
                result.outer[0] = (vy);
                result.inner[1] = (vx);
                result.inner[2] = (vx);
                h_max = a;
            }
        }
    }
    std::free(s);
    return result;
}
