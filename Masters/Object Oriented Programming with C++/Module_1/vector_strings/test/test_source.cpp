#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>

#include <gcheck.h>
#include <argument.h>
#include <redirectors.h>
#include "../src/vector_strings.hpp"

PREREQ_TEST(VectorStrings, Add, 1, 1) {
    GradingMethod(gcheck::AllOrNothing);
    
    std::vector<std::string> v;

    gcheck::StdinInjecter tin("Esko\n");
    gcheck::StdoutCapturer tout;

    Adder(v);
    
    tout.Restore();
    std::string Adder_out = tout.str();

    std::string correct_answer = "Enter a name:\nNumber of names in the vector:\n1\n";
    EXPECT_EQ(correct_answer, Adder_out);

    ASSERT_TRUE(v.size() == 1);
    EXPECT_EQ("Esko", v[0]);
}

TEST(test_vector_strings, test_remove){
    GradingMethod(gcheck::AllOrNothing);
    
    std::vector<std::string> v;

    gcheck::StdinInjecter tin("Esko\nEero\nFranz\n");

    Adder(v);
    Adder(v);
    Adder(v);
    
    gcheck::StdoutCapturer tout;

    Remover(v);

    tin.Restore();
    tout.Restore();
    std::string Remover_out = tout.str();
    
    std::string correct_answer = "Removing the last element:\nFranz\n";
    EXPECT_EQ(correct_answer, Remover_out);

    ASSERT_TRUE(v.size() == 2);
    EXPECT_EQ("Esko", v[0]);
    EXPECT_EQ("Eero", v[1]);
}

TEST(test_vector_strings, test_print){
    GradingMethod(gcheck::AllOrNothing);
    
    std::vector<std::string> v;

    gcheck::StdinInjecter tin("Eero\nEsko\nFranz\n");

    Adder(v);
    Adder(v);
    Adder(v);
    
    gcheck::StdoutCapturer tout;
    
    Printer(v);

    tin.Restore();
    tout.Restore();
    std::string Printer_out = tout.str();
    
    std::string correct_answer = "Eero\nEsko\nFranz\n";
    EXPECT_EQ(correct_answer, Printer_out);

    ASSERT_TRUE(v.size() == 3);
    EXPECT_EQ("Eero", v[0]);
    EXPECT_EQ("Esko", v[1]);
    EXPECT_EQ("Franz", v[2]);
}


TEST(VectorStrings, CMDReader) {
    
    std::string input = "ADD\nEsa\nPRINT\nREMOVE\nQUIT\n";
    gcheck::StdinInjecter tin(input);
    tin.Close();
    
    gcheck::StdoutCapturer tout;
    
    CMDReader();
    
    tin.Restore();
    tout.Restore();
    
    std::string CMDReader_Out = tout.str();

    std::string correct_answer = "Commands: ADD, PRINT, REMOVE, QUIT\n"
        "Enter a command:\nEnter a name:\nNumber of names in the vector:\n1\n"
        "Enter a command:\nEsa\n"
        "Enter a command:\nRemoving the last element:\nEsa\n"
        "Enter a command:\n";
    EXPECT_EQ(correct_answer, CMDReader_Out) << "Input:\n" << input;
    
    
    input = "ADD\nEsko\nADD\nErkki\nREMOVE\nADD\nJaana\nPRINT\nQUIT\n";
    tin.Write(input);
    tin.Close();
    
    tout.Capture();
    
    CMDReader();

    tout.Restore();
    
    CMDReader_Out = tout.str();

    correct_answer = "Commands: ADD, PRINT, REMOVE, QUIT\n"
        "Enter a command:\nEnter a name:\nNumber of names in the vector:\n1\n"
        "Enter a command:\nEnter a name:\nNumber of names in the vector:\n2\n"
        "Enter a command:\nRemoving the last element:\nErkki\n"
        "Enter a command:\nEnter a name:\nNumber of names in the vector:\n2\n"
        "Enter a command:\nEsko\nJaana\n"
        "Enter a command:\n";
    EXPECT_EQ(correct_answer, CMDReader_Out)  << "Input:\n" << input;
}
