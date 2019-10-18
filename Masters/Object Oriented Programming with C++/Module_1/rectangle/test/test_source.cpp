#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include <gcheck.h>
#include <argument.h>
#include <redirectors.h>
#include "../src/rectangle.hpp"

std::string TRectangle(std::string input) {
    
    gcheck::StdinInjecter tin(input);
    gcheck::StdoutCapturer tout;
    
    Rectangle();
    
    return tout.str();
}


TEST(Rectangle, Test) {
    {
        std::vector correct_not_reserved = {"Please enter width and height\nArea: 556.043\nCircumference: 194.545\n","Please enter width and height\nArea: 2421.09\nCircumference: 196.851\n","Please enter width and height\nArea: 2886.72\nCircumference: 232.413\n","Please enter width and height\nArea: 2481.03\nCircumference: 203.165\n","Please enter width and height\nArea: 7309.6\nCircumference: 342.017\n"};
        gcheck::SequenceArgument inputs_not_reserved0({std::string("6.098704 91.173889\n"),std::string("48.323013 50.102270\n"),std::string("35.984001 80.222256\n"),std::string("60.726404 40.855930\n"),std::string("86.681747 84.326894\n")});
        TestCase(5,correct_not_reserved,TRectangle,inputs_not_reserved0);
    };
}
