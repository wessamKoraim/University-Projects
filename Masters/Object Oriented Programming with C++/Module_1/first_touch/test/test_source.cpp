#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

#include <gcheck.h>
#include <redirectors.h>
#include "../src/first.hpp"

TEST(FirstTouch, Hello) {
    gcheck::StdoutCapturer tout;

    Hello();
    
    tout.Restore();
    std::string Hello_out = tout.str();

    std::cout << "Testing output" << std::endl;
    std::cout << Hello_out << std::endl;
    
    std::string correct_answer = "Hello world!\n";
    EXPECT_EQ(correct_answer, Hello_out);
}

