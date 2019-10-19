#include "so.h"
#include <algorithm>
#include <omp.h>
//using namespace std;

void quickSort(data_t * first , data_t * last){

	int size = last - first;
	if( size < 7000 ){
		std::sort(first , last);
	} else {
		data_t * p = last - 1;
		std::swap( * ( first + size / 2 ) , * p);
		data_t * q = std::partition(first , p , [p](data_t v) -> bool {return v < * p;});
		std::swap(*q , *p);
		#pragma omp task
		quickSort( first , q );
		#pragma omp task
		quickSort( q + 1 , last );
	}
}

void psort(int n, data_t * data){
	srand(time(NULL));
	#pragma omp parallel num_threads(omp_get_max_threads())
	#pragma omp single
	{
		quickSort(data , data + n);
	}
}
