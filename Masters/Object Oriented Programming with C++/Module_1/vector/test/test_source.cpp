#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <numeric>

#include <gcheck.h>
#include <argument.h>
#include "../src/vector.hpp"

auto TMin(const std::vector<int>& items) {
    return GetMin(items);
}
TEST(Vector, Min) {
    {
        std::vector correct_not_reserved = {-36,-78,-54,-93,-57};
        gcheck::SequenceArgument inputs_not_reserved0({std::vector<int>({-36}),std::vector<int>({57, 74, -78, 79}),std::vector<int>({59, 42, 14, 45, -8, -44, 93, -54, 74}),std::vector<int>({99, -13, 9, 87, -27, -93}),std::vector<int>({69, 4, -57, 43, -4, 32})});
        TestCase(5,correct_not_reserved,TMin,inputs_not_reserved0);
    };
}

auto TMax(const std::vector<int>& items) {
    return GetMax(items);
}
TEST(Vector, Max) {
    {
        std::vector correct_not_reserved = {100,95,84,72,93};
        gcheck::SequenceArgument inputs_not_reserved0({std::vector<int>({100}),std::vector<int>({-61, 43, 95, -43, 50, -57, -87, -39, -74}),std::vector<int>({84, 76, -12, -23, -63, -17}),std::vector<int>({72, 8, -27, -15, -81, -40, -97, -74, -48, 11}),std::vector<int>({28, -32, 1, 93, 68, -14})});
        TestCase(5,correct_not_reserved,TMax,inputs_not_reserved0);
    };
}

std::string TAverage(const std::vector<int>& items) {
    return std::to_string(GetAvg(items));
}
TEST(Vector, Average) {
    {
        std::vector correct_not_reserved = {"0.800000","44.000000","3.666667","-1.333333","-36.000000"};
        gcheck::SequenceArgument inputs_not_reserved0({std::vector<int>({-22, -8, 3, 40, 61, -43, 55, -42, 54, -90}),std::vector<int>({44}),std::vector<int>({-84, -14, 79, 19, 76, 41, -34, -21, -29}),std::vector<int>({81, 0, -85}),std::vector<int>({-36})});
        TestCase(5,correct_not_reserved,TAverage,inputs_not_reserved0);
    };
}
