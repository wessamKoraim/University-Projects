#include "dog_class.hpp"

//Definitions of class functions with some errors


Dog::Dog(int a, std::string n) { 
    age_ = a; 
    name_ = n; 
}


void Dog::SetAge(int age) {
    age_ = age; 
}


int Dog::GetAge() const{
    return age_; 
}


void Dog::SetName(std::string n) {
    name_ = n; 
}


std::string Dog::GetName() const {
    return name_; 
}


