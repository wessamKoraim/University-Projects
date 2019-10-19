#include "so.h"
#include <algorithm>
#include <iostream>
#include <omp.h>
#include <math.h>
using namespace std;

void psort(int n, data_t* data) 
{
    int p = omp_get_max_threads();

    int pr = p == 1 ? 2 : 2 << ( (int)(log2(p))-1);
    int block = n / pr;

    if( (n < pr) || (block == 0) )
    {
        sort( data, data + n );
    }
    else
    {
        int nr = pr * block;//n rounded 
        #pragma omp parallel for num_threads( pr )
        for( int i = 0; i < pr; i++ )
        {
            sort( data + i * block, data + ( i + 1 ) * block );
            if( i == pr - 1 )
            {
                sort( data + nr, data + n );
            }
        }

        int lg = log2(nr/block);
        for( int i = 0; i < lg; i++ )

        {
            int tpi = 2 << i;
            #pragma omp parallel for num_threads( pr ) schedule( static, 1 )
            for( int j = 0; j < pr; j += tpi )
            {
                int f = j * block;
                int t = f + tpi * block;
                int m = f + ( ( t - f ) >> 1 );

                inplace_merge( data + f , data + m, data + t );

                if( j + tpi  == pr )
                {
                    inplace_merge( data + f, data + nr, data + n );
                }
            }
        }
    }
}

