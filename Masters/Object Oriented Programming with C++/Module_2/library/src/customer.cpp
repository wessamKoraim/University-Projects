#include "customer.hpp"


Customer::Customer(std::string const& name, std::string const& customer_id)
{
    name_ = name;
    customer_id_ = customer_id;
}

std::string Customer::GetName(void) const
{
    return name_;
}

std::string Customer::GetID(void) const
{
    return customer_id_;
}

int Customer::GetLoanAmount() const
{
    return loaned_books_.size();
}

std::vector<Book> Customer::GetLoans(void) const
{
    return loaned_books_;
}

bool Customer::LoanBook(Book& book)
{
    bool RetVal = false;

    if(book.Loan())
    {
        RetVal = true;
        loaned_books_.emplace_back(book);
    }
    return RetVal;
}

void Customer::ReturnBook(Book& book)
{
    int i = 0;
    for(auto elem : loaned_books_)
    {
        

        if(elem.GetName() == book.GetName())
        {
            loaned_books_.erase( loaned_books_.begin() + i );
            break;
        }

        i++;
    }

    book.Restore();
}

void Customer::Print(void) const
{
    std::cout << "Customer: "<< name_;
    std::cout <<", "<< customer_id_;
    std::cout <<", has "<< loaned_books_.size() <<" books on loan:" << std::endl;

    for(auto & elem: loaned_books_)
    {
        std::cout << "- ";
        elem.Print();
    }
}

