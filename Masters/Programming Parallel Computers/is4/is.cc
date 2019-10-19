#include "is.h"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <omp.h>
using namespace std;
typedef double double4_t __attribute__ ((vector_size (4 * sizeof(double))));

typedef struct
{
    double h;
    Result r;
}HelperStruct;

Result segment(int ny, int nx, const float* data) 
{
    Result result { 0, 0, 0, 0, {0, 0, 0}, {0, 0, 0} };

    int sum_nx = nx+1;
    int sum_ny = ny+1;
    double4_t s[ sum_nx * sum_ny ];

    for(int x = 0; x < sum_nx; x++ )
    {
        s[x][0]=0;
        s[x][1]=0;
        s[x][2]=0;
    }
    for(int y = 0; y < sum_ny; y++ )
    {
        s[sum_nx*y][0]=0;
        s[sum_nx*y][1]=0;
        s[sum_nx*y][2]=0;
    }

    s[ 1 + sum_nx][0] = (double)data[0];
    s[ 1 + sum_nx][1] = (double)data[1];
    s[ 1 + sum_nx][2] = (double)data[2];

    HelperStruct Helper[ ny ];
    #pragma omp parallel for
    for( int c = 0; c < 3; c++ )
    {
        for( int x = 3; x < 3 * nx; x += 3 )
        {
            s[ 1 + x/3 + sum_nx ][c] = (double)data[ c + x ] + s[1 + x/3 - 1 + sum_nx ][c];
        }
        for ( int y = 2; y < sum_ny; y++ )
        {
            for ( int x = 1; x < sum_nx; x++ )
            {
                int ind  =  x + sum_nx * y;          
                int ind3 =  x + sum_nx * ( y - 1 );  
                if( x == 0 )
                {
                    s[ ind ][ c ] = (double)data[ c + 3 * (x-1) + 3 * nx * (y-1) ] + s[ ind3 ][ c ];
                }
                else
                {
                    int ind2 = ( x -1 ) + sum_nx * y;    
                    int ind4 = ( x -1 ) + sum_nx * (y-1);
                    s[ ind ][ c ] = (double)data[ c + 3 * (x-1) + 3 * nx * (y-1)  ] + s[ ind2 ][ c ] - s[ ind4 ][ c ] + s[ ind3 ][ c ];
                }  
            }
        }
    }

    double h = 0;

    const double4_t sy = s[ ( sum_nx - 1 )  + sum_nx * ( sum_ny -1 ) ];
    #pragma omp parallel for schedule (dynamic,1) num_threads( omp_get_max_threads())
    for( int len_y = 1; len_y <= ny; len_y++ )
    {
        Result LoopResult;
        double tmp = 0;
        for( int len_x = 1; len_x <= nx; len_x++ )
        {
            double X = (double) (len_x  * len_y);
            double Y = (double) (nx * ny - X);
            double tmp_X = 1/X;
            double tmp_Y = 1/Y;
            double4_t X_inv= {tmp_X,tmp_X,tmp_X,tmp_X};
            double4_t Y_inv={tmp_Y,tmp_Y,tmp_Y,tmp_Y};
            for( int pos_y = 1; pos_y <= sum_ny - len_y; pos_y++ )
            {
                for( int pos_x = 1; pos_x <= sum_nx - len_x; pos_x++ )
                {
                    int x1 = pos_x + len_x - 1;
                    int y1 = pos_y + len_y - 1;
                    double4_t vx;
                    double4_t vy;
                    double h_loop = 0;
                    vx =    s[ x1 + sum_nx * y1 ] - 
                            s[  x1 + sum_nx * ( pos_y - 1 ) ] - 
                            s[  ( pos_x - 1 ) + sum_nx * y1 ] + 
                            s [  ( pos_x - 1 ) + sum_nx * ( pos_y - 1) ];
                    vy = sy - vx;
                    double4_t a = vx * vx / X;
                    double4_t b = vy * vy / Y;
                    for( int c = 0; c < 3; ++ c )
                    {
                        h_loop += ( a[c] + b[c] );
                    }
                    vx *= X_inv;
                    vy *= Y_inv;

                    if( h_loop > tmp )
                    {
                        LoopResult.x0 = pos_x - 1;
                        LoopResult.x1 = x1;
                        LoopResult.y0 = pos_y - 1;
                        LoopResult.y1 = y1;
                        LoopResult.outer[ 0 ] = (float)(vy[ 0 ]);
                        LoopResult.outer[ 1 ] = (float)(vy[ 1 ]);
                        LoopResult.outer[ 2 ] = (float)(vy[ 2 ]);
                        LoopResult.inner[ 0 ] = (float)(vx[ 0 ]);
                        LoopResult.inner[ 1 ] = (float)(vx[ 1 ]);
                        LoopResult.inner[ 2 ] = (float)(vx[ 2 ]);
                        tmp = h_loop;
                    }
                }
                
            }
        }
        Helper[len_y - 1].r = LoopResult;
        Helper[len_y - 1].h = tmp;
    }
        for( int y = 0; y < ny; y++ )
        {
            if( Helper[y].h > h )
            {
                h = Helper[y].h;
                result = Helper[y].r;
            }
        }
    
    return result;
}



