#include <iostream>
#include <vector>
#include "vector_it.hpp"

// Implement the functions here


std::vector<int> ReadVector(void)
{
    std::vector<int> RetVal;
    int number;
    
    while (std::cin >> number)
        RetVal.emplace_back(number);
    
    return RetVal;
}

void PrintSum1(std::vector<int>& InputVector)
{
    for(unsigned int LoopIndex = 0; LoopIndex < InputVector.size() - 1; LoopIndex++)
    {   
        std::cout << InputVector[LoopIndex] + InputVector[LoopIndex + 1] << ' ';
    }

    std::cout << std::endl;
}

void PrintSum2(std::vector<int>& InVec)
{
    auto HeadIt = InVec.begin();
    auto TailIt = InVec.end();
    auto Size   = InVec.size()/2;

    TailIt--;

    while(Size != 0)
    {
        std::cout << *HeadIt + *TailIt << ' ';
        HeadIt++;
        TailIt--;
        Size--;
    }

    std::cout << std::endl;
}