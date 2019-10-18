#include <argument.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include <gcheck.h>
#include "../src/person.hpp"

struct Res {
    int age;
    std::string name;
};
std::string to_string(const Res& res) {
    return std::string("Age: ") + std::to_string(res.age) + ", Name: " + res.name; 
}

bool operator==(const Res& l, const std::string& r) {
    return to_string(l) == r;
}

Res TestConstruct(int birthyear, std::string name, int current_year) {
    Person person(name, birthyear);
    return Res({person.GetAge(current_year), person.GetName()});
}

TEST(SimpleClass, Test) {
    gcheck::Random rnd_year(1950, 2010);
    gcheck::Random rnd_current_year(2010, 2020);
    gcheck::Random rnd_name(std::vector<std::string>{"Kalle Kankkunen", "Ville Vinkkunen", "Liisa Linkkunen", "Silli Sinkkunen"});
    
    {
        std::vector correct_not_reserved = {"Age: 7, Name: Ville Vinkkunen","Age: 43, Name: Liisa Linkkunen","Age: 17, Name: Ville Vinkkunen","Age: 61, Name: Silli Sinkkunen","Age: 66, Name: Kalle Kankkunen"};
        gcheck::SequenceArgument inputs_not_reserved0({2010,1976,1996,1952,1954});
        gcheck::SequenceArgument inputs_not_reserved1({std::string("Ville Vinkkunen"),std::string("Liisa Linkkunen"),std::string("Ville Vinkkunen"),std::string("Silli Sinkkunen"),std::string("Kalle Kankkunen")});
        gcheck::SequenceArgument inputs_not_reserved2({2017,2019,2013,2013,2020});
        TestCase(5,correct_not_reserved,TestConstruct,inputs_not_reserved0,inputs_not_reserved1,inputs_not_reserved2);
    };
}
