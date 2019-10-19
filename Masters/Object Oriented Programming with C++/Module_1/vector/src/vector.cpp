#include "vector.hpp"

#include <vector>

int GetMin(std::vector<int> v) {
    
    int Size = v.size();
    int Min = v[0];

    for(int i = 1; i < Size; i++){
        if ( Min > v[i] ){
            Min = v[i];
        }
    }
    return Min; 
}

int GetMax(std::vector<int> v) {
    int Size = v.size();
    int Max = v[0];

    for(int i = 1; i < Size; i++){
        if ( Max < v[i] ){
            Max = v[i];
        }
    }
    return Max; 
}

double GetAvg(std::vector<int> v) {
    
    double Sum = 0;
    int Size = v.size();

    for(int i = 0; i < Size; i++){
        Sum += (double) v[i];
    }

    return (Sum / (double) Size);
}
