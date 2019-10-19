#include "person.hpp"

// define your Person class' functions here

Person::Person(std::string name, int BirthYear)
{
    Name = name;
    birthyear = BirthYear;
}

std::string Person::GetName(void) const
{
    return Name;
}

int Person::GetAge(int CurrentYear) const
{
    return (CurrentYear - birthyear);
}