#include <iostream>
#include "book.hpp"
#include "customer.hpp"
#include "library.hpp"


Library::Library(std::string const& name)
{
    name_ = name;
}

std::string Library::GetName(void) const
{
    return name_;
}

std::vector<Book>& Library::GetBooks(void)
{
    return books_;
}

std::vector<Customer>& Library::GetCustomers(void)
{
    return customers_;
}

Book Library::FindBookByName(std::string const& book_name)
{
    Book FoundBook("err", "err", "err");
    for(auto elem: books_)
    {
        if(elem.GetName() == book_name)
        {
            FoundBook = elem;
        }
    }
    return FoundBook;
}

std::vector<Book> Library::FindBooksByAuthor(std::string const& author_name)
{
    std::vector<Book> FoundBooks;
    for(auto elem: books_)
    {
        if(elem.GetAuthor() == author_name)
        {
            FoundBooks.emplace_back(elem);
        }
    }
    return FoundBooks;
}

std::vector<Book> Library::FindAllLoanedBooks()
{
    std::vector<Book> FoundBooks;
    for(auto elem: books_)
    {
        if(elem.GetStatus())
        {
            FoundBooks.emplace_back(elem);
        }
    }
    return FoundBooks;
}

Customer Library::FindCustomer(std::string const& customer_id)
{
    Customer FoundCustomer("err", "err");
    for(auto elem: customers_)
    {
        if(elem.GetID() == customer_id)
        {
            FoundCustomer = elem;
        }
    }
    return FoundCustomer;
}
