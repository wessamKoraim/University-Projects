#include "book.hpp"

#include <ctime>
#include <iostream>


Book::Book(std::string const& name,
    std::string const& author,
    std::string const& isbn,
    bool loaned,
    Date due_date )
{
    name_       = name;
    author_     = author;
    isbn_       = isbn;
    loaned_     = loaned;
    due_date_   = due_date;
}

std::string Book::GetName() const
{
    return name_;
}

std::string Book::GetAuthor() const
{
    return author_;
}

std::string Book::GetISBN() const
{
    return isbn_;
}

bool Book::GetStatus() const
{
    return loaned_;
}

Date Book::GetDueDate() const
{
    return due_date_;
}

bool Book::Loan(void)
{
    bool RetVal = false;

    if(!loaned_)
    {
        RetVal = true;
        due_date_ = Date::Today();
        due_date_.month++;
        loaned_ = true;
    }

    return RetVal;    
}

void Book::Restore(void)
{
    due_date_ = {0, 0, 0};
    loaned_ = false;
}

void Book::Print(void) const
{
    std::cout << "Book: "<< name_ <<", author: "<< author_;
    std::cout <<", ISBN: "<< isbn_ <<", loaned " << (loaned_ ? "true":"false");
    std::cout << ", due date: "<< due_date_.day <<".";
    std::cout << due_date_.month <<"."<< due_date_.year << std::endl;
}