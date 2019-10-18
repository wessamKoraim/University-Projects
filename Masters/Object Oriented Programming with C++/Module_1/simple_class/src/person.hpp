#ifndef AALTO_ELEC_CPP_PERSON
#define AALTO_ELEC_CPP_PERSON

// define your Person class here
#include <string>

class Person
{
    private:
        std::string Name;
        int birthyear;

    public:
        Person(std::string name, int BirthYear);

        std::string GetName(void) const;

        int GetAge(int CurrentYear) const;
};

#endif