#include "mf.h"
//#include <iostream>
#include <vector>
#include <algorithm>


inline float median(std::vector<float>& p_values)
{
    std::size_t n = p_values.size() / 2;
    //std::cout<<"here"<<std::endl;
    nth_element( p_values.begin() , p_values.begin() + n , p_values.end() );
    float mid = p_values.at(n);
    if( !(p_values.size() & 1) )
    {
    	nth_element(p_values.begin(), p_values.begin()+n-1, p_values.end());
    	mid = ( mid + p_values.at(n-1) ) / 2;
    }
    return mid;
}

inline float Median( int& i, int& j, int& hy, int& hx, int& ny, int& nx, const float * Data )
{
    int Left  = ( i - hx  <= 0  ) ? 0 : i - hx;
    int Right = ( i + hx  >= nx ) ? nx-1 : i + hx;
    int Upper = ( j - hy  <= 0  ) ? 0 : j - hy;
    int Lower = ( j + hy  >= ny ) ? ny-1 : j + hy;
	
    //std::cout<<std::endl;
    
    //std::cout<<"( Lower - Upper + 1 ) * ( Right - Left + 1 ) = "<<( Lower - Upper + 1 ) * ( Right - Left + 1 )<<std::endl;
    //std::vector<float> values( ( Lower - Upper + 1 ) * ( Right - Left + 1 ) );
    std::vector<float> values((Lower - Upper + 1) * ( Right - Left + 1 ) );
    //std::cout<<"here"<<std::endl;
    //p_values->resize(( Lower - Upper + 1 ) * ( Right - Left + 1 ));
    //std::cout<<"size of vec = "<<p_values->size()<<std::endl;
    float m;
    int cntr = 0;

	for( auto y = Upper; y <= Lower; y++ )
	{
		for( auto x = Left; x <= Right; x++  )
		{
			//values[ (y - Upper) * nx + (x - Left) ] = (Data[y * nx + x]);
                        //std::cout<<"cntr = "<<cntr<<std::endl;
                        //std::cout<<"data index = "<<x + y * nx<<std::endl;
		        //std::cout<<"here"<<std::endl;
                        values[cntr++] = (Data[x + y * nx]);
		}
                //std::cout<<"y = "<<y<<std::endl;
	}
        do{}while(0);
        m = median(values);
        //delete p_values;
	return m;
}

void mf(int ny, int nx, int hy, int hx, const float* in, float* out) 
{
    #pragma omp parallel for schedule(static , 1)   
    for(auto y = 0; y < ny; y++ )
    {
        #pragma omp parallel for schedule(static , 1)
    	for(auto x = 0; x < nx; x++ )
    	{
    		out[ x + y * nx ] = Median( x , y , hy , hx , ny , nx , in );
    	}
    }
    return;
}


