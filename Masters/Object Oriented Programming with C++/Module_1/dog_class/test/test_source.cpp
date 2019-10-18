#include <argument.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include <gcheck.h>
#include "../src/dog_class.hpp"

struct Res {
    int age;
    std::string name;
};
std::string to_string(const Res& res) {
    return std::string("Name: ") + res.name + ", Age: " + std::to_string(res.age); 
}

bool operator==(const Res& l, const std::string& r) {
    return to_string(l) == r;
}

Res TestConstruct(int age, std::string name) {
    Dog dog(age, name);
    return Res({dog.GetAge(), dog.GetName()});
}

Res TestSet(int age, std::string name) {
    Dog dog(0, "");
    dog.SetAge(age);
    dog.SetName(name);
    
    return Res({dog.GetAge(), dog.GetName()});
}

TEST(DogClass, constructor) {
    gcheck::Random rnd(1, 100);
    gcheck::SequenceArgument<std::string> seq({"Max", "Buddy"});
    
    {
        std::vector correct_not_reserved = {"Name: Max, Age: 24","Name: Buddy, Age: 83"};
        gcheck::SequenceArgument inputs_not_reserved0({24,83});
        gcheck::SequenceArgument inputs_not_reserved1({std::string("Max"),std::string("Buddy")});
        TestCase(2,correct_not_reserved,TestConstruct,inputs_not_reserved0,inputs_not_reserved1);
    };
}

TEST(DogClass, SetAge_SetName) {
    gcheck::Random rnd(1, 100);
    gcheck::SequenceArgument<std::string> seq({"Mr. Wuf", "Doge"});
    
    {
        std::vector correct_not_reserved = {"Name: Mr. Wuf, Age: 31","Name: Doge, Age: 9"};
        gcheck::SequenceArgument inputs_not_reserved0({31,9});
        gcheck::SequenceArgument inputs_not_reserved1({std::string("Mr. Wuf"),std::string("Doge")});
        TestCase(2,correct_not_reserved,TestSet,inputs_not_reserved0,inputs_not_reserved1);
    };
}
