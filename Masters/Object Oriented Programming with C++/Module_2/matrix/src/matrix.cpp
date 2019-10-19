#include <iostream>
#include <vector>
#include "matrix.hpp"

// Implement the functions here
// Remember to use auto and ranged for-loops when they can be used!


Matrix ReadMatrix(int n)
{
    Matrix InMatrix(n);
    
    for(auto i = 0; i < n; i++)
        InMatrix[i].resize(n);

    for(auto row = 0; row < n; row++)
        for(auto col = 0; col < n; col++)
            std::cin >> InMatrix[row][col];

    return InMatrix;
}

Matrix Rotate90Deg(Matrix & InMat)
{
    
    int MatSize = (int) InMat.size();
    std::cout << "SIZE " << MatSize << std::endl;
    int InMatRow = 0;
    int InMatCol = 0;

    Matrix RetMat(MatSize);

    for(auto i = 0; i < MatSize; i++)
        RetMat[i].resize(MatSize);

    for(auto RetMatCol = MatSize - 1; RetMatCol >= 0 ; RetMatCol--)
    {
        InMatCol = 0;

        for(auto RetMatRow = 0; RetMatRow < MatSize ; RetMatRow++)
        {
            RetMat[RetMatRow][RetMatCol] = InMat[InMatRow][InMatCol];
            //std::cout << "InMatRow: " << InMatRow << " InMatCol: " << InMatCol << std::endl;
            //std::cout << "RetMatRow: " << RetMatRow << " RetMatCol: " << RetMatCol << std::endl;
            InMatCol++;
        }
        
        InMatRow++;
    }

    return RetMat;
}

void Print(Matrix & InMat)
{
    int n = (int) InMat.size();
    std::cout << "Printing out a " << n << " x " << n << " matrix:" << std::endl;
    
    for(auto row = 0; row < n; row++){
        for(auto col = 0; col < n; col++){
            std::cout << InMat[row][col] << ' ';
        }
        std::cout << std::endl;
    }
}