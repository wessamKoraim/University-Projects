#include "vector_strings.hpp"

#include <iostream>
#include <vector>

void Adder(std::vector<std::string>& names) {
    std::string Name;
    std::cout << "Enter a name:" << std::endl;
    std::cin >> Name;
    names.emplace_back(Name);

    std::cout << "Number of names in the vector:\n" << names.size() << std::endl;

    
}

void Remover(std::vector<std::string>& names) {
    std::cout << "Removing the last element:\n" << names[names.size() - 1] << std::endl;
    names.pop_back();
}

void Printer(std::vector<std::string>& names) {
    for(unsigned int i = 0; i < names.size(); i++){
        std::cout << names[i] << std::endl;
    }
}

void CMDReader() {
    std::string Input;
    std::vector<std::string> names;
    std::cout << "Commands: ADD, PRINT, REMOVE, QUIT" << std::endl;

    while(1){

        std::cout << "Enter a command:" << std::endl;
        std::cin >> Input;

        if(!Input.compare("ADD")){
            Adder(names);
        }
        else if(!Input.compare( "PRINT" )){
            Printer(names);
        }
        else if(!Input.compare( "REMOVE" )){
            Remover(names);
        }
        else if(!Input.compare( "QUIT" )){
            return;
        }
    }
}

