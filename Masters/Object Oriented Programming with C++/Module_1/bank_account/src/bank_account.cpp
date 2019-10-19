#include "bank_account.hpp"

BankAccount::BankAccount(   std::string const& owner,
                            std::string const& account_number,
                            double balance)
{
    owner_ = owner;
    account_number_ = account_number;
    balance_ = balance;
}

BankAccount::BankAccount(   std::string const& owner,
                            std::string const& account_number)
{
    owner_ = owner;
    account_number_ = account_number;
    balance_ = 0.0;
}

std::string BankAccount::GetOwner() const
{
    return owner_;
}

std::string BankAccount::GetNumber() const
{
    return account_number_;
}

double BankAccount::GetBalance() const
{
    return balance_;
}

void BankAccount::Deposit(double amount)
{
    balance_ += amount;
}

bool BankAccount::Withdraw(double amount)
{
    bool RetVal = false;

    if( amount <= balance_ )
    {
        balance_ -= amount;
        RetVal = true;
    }

    return RetVal;
}

void BankAccount::Print() const
{
    std::cout << "Account num: " << account_number_ <<", owned by: "<< owner_ <<", balance: " << balance_ << " eur.\n";
}

bool Transfer(BankAccount& source_account, BankAccount& target_account, double amount)
{
    bool RetVal = false;

    if( source_account.Withdraw(amount) )
    {
        target_account.Deposit(amount);
        RetVal = true;
    }

    return RetVal;
}
