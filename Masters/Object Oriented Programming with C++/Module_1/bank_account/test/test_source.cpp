#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cstddef>

#include <gcheck.h>
#include <redirectors.h>
#include "../src/bank_account.hpp"

TEST(BankAccount, constructor) {
    BankAccount("Roope Rikas", "12345", 100.0);
    BankAccount("Kaisa Köyhä", "53457");
}

TEST(BankAccount, GetOwner) {
    BankAccount roope_account("Roope Rikas", "12345", 100.0);
    BankAccount kaisa_account("Kaisa Köyhä", "53457");
    
    EXPECT_EQ("Roope Rikas", roope_account.GetOwner());
    EXPECT_EQ("Kaisa Köyhä", kaisa_account.GetOwner());
}

TEST(BankAccount, GetNumber) {
    srand(time(0));
    std::string number1 = std::to_string(static_cast<long int> (rand() / (RAND_MAX)));
    std::string number2 = std::to_string(static_cast<long int> (rand() / (RAND_MAX)));
    BankAccount roope_account("Roope Rikas", number1, 100.0);
    BankAccount kaisa_account("Kaisa Köyhä", number2);
    
    EXPECT_EQ(number1, roope_account.GetNumber());
    EXPECT_EQ(number2, kaisa_account.GetNumber());
}

TEST(BankAccount, GetBalance) {
    srand(time(0));
    auto amount1 = rand() / (RAND_MAX / 100.0);
    auto amount2 = rand() / (RAND_MAX / 100.0);
    BankAccount roope_account("Roope Rikas", "12345", amount1);
    BankAccount kaisa_account("Kaisa Köyhä", "53457", amount2);
    
    EXPECT_EQ(amount1, roope_account.GetBalance());
    EXPECT_EQ(amount2, kaisa_account.GetBalance());
}

TEST(BankAccount, Deposit) {
    srand(time(0));
    auto amount1 = rand() / (RAND_MAX / 100.0);
    auto amount2 = rand() / (RAND_MAX / 100.0);
    BankAccount roope_account("Roope Rikas", "12345", amount1);
    BankAccount kaisa_account("Kaisa Köyhä", "53457", amount2);

    auto deposit1 = rand() / (RAND_MAX / 100.0);
    auto deposit2 = rand() / (RAND_MAX / 100.0);

    roope_account.Deposit(deposit1);
    kaisa_account.Deposit(deposit2);

    EXPECT_EQ(amount1+deposit1, roope_account.GetBalance());
    EXPECT_EQ(amount2+deposit2, kaisa_account.GetBalance());
}

TEST(BankAccount, Withdraw) {
    srand(time(0));
    auto amount1 = rand() / (RAND_MAX / 100.0);
    auto amount2 = rand() / (RAND_MAX / 100.0);
    BankAccount roope_account("Roope Rikas", "12345", amount1);
    BankAccount kaisa_account("Kaisa Köyhä", "53457", amount2);

    auto withdraw1 = rand() / (RAND_MAX / 100.0);
    while(withdraw1 >= amount1)
        withdraw1 = rand() / (RAND_MAX / 100.0);
 
    auto withdraw2 = rand() / (RAND_MAX / 100.0);
    while(withdraw2 >= amount2)
        withdraw2 = rand() / (RAND_MAX / 100.0);
    

    roope_account.Withdraw(withdraw1);
    kaisa_account.Withdraw(withdraw2);

    EXPECT_EQ(amount1-withdraw1, roope_account.GetBalance());
    EXPECT_EQ(amount2-withdraw2, kaisa_account.GetBalance());

    roope_account.Withdraw(roope_account.GetBalance()+100.0);
    EXPECT_EQ(amount1-withdraw1, roope_account.GetBalance());
}

TEST(BankAccount, Transfer) {
    srand(time(0));
    auto amount1 = rand() / (RAND_MAX / 100.0);
    auto amount2 = rand() / (RAND_MAX / 100.0);
    BankAccount roope_account("Roope Rikas", "12345", amount1);
    BankAccount kaisa_account("Kaisa Köyhä", "53457", amount2);

    auto withdraw = rand() / (RAND_MAX / 100.0);
    while(withdraw >= amount1)
        withdraw = rand() / (RAND_MAX / 100.0);

    Transfer(roope_account, kaisa_account, withdraw);

    EXPECT_EQ(amount1-withdraw, roope_account.GetBalance());
    EXPECT_EQ(amount2+withdraw, kaisa_account.GetBalance());

    Transfer(roope_account, kaisa_account, roope_account.GetBalance()+100.0);
    EXPECT_EQ(amount1-withdraw, roope_account.GetBalance());
    EXPECT_EQ(amount2+withdraw, kaisa_account.GetBalance());
}

TEST(BankAccount, Print) {
    //create two random numbers
    srand(time(0));
    auto amount1 = rand() / (RAND_MAX / 100.0);
    auto amount2 = rand() / (RAND_MAX / 100.0);
    BankAccount roope_account("Roope Rikas", "12345", amount1);
    BankAccount kaisa_account("Kaisa Köyhä", "53457", amount2);

    gcheck::StdoutCapturer tout;

    roope_account.Print();
    
    tout.Restore();
    std::string Print_out = tout.str();

    std::cout << "Testing output" << std::endl;
    std::ostringstream c;
    c << "Account num: 12345, owned by: Roope Rikas, balance: " << amount1 << " eur.\n";
    std::string correct = c.str();

    std::cout << Print_out << std::endl;
    EXPECT_EQ(correct, Print_out);
    
    c.str("");
    c.clear();

    tout.Capture();
    
    kaisa_account.Print();
    
    tout.Restore();
    Print_out = tout.str();
    
    c << "Account num: 53457, owned by: Kaisa Köyhä, balance: " << amount2 << " eur.\n";
    correct = c.str();
    std::cout << Print_out << std::endl;
    EXPECT_EQ(correct, Print_out);
} 
