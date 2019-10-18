#include "list.hpp"
#include <iostream>

// Implement the functions here

std::istream& GetLines(std::istream& is, std::list<std::string>& list)
{
    char line[1000];
    //is.getline(line, 1000);
    //list.emplace_back(line);
    

    while (is.getline(line, 1000))
    {
        if(!(is.eof()))
        {   
            list.emplace_back(line);
        }
    }


    return is;
}

void Print(const std::list<std::string>& list)
{
    
    for(auto it = list.begin(); it != list.end(); it++)
    {
        std::cout << *it << std::endl;
    }
}

void SortAndUnique(std::list<std::string>& list)
{
    list.sort();
    list.unique();
}