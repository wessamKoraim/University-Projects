#ifndef ELEC_AS_CPP_LIBRARY
#define ELEC_AS_CPP_LIBRARY

#include <string>
#include <vector>
#include "book.hpp"
#include "customer.hpp"

class Library {
public:
    /* Library:
    * the constructor of the Library class, takes following parameters:
    *  - the library's name (const reference to string)
    * Remember to also initialize books and customers in the constructor!
    */
    
   Library(std::string const& name);
    /* GetName:
    * returns the library's name as a string, takes no parameters.
    * This function should not alter the Library object's state, in other words the function should be const.
    */
    
    std::string GetName(void) const;

    /* GetBooks:
    * returns the Library's books as a reference to a vector<Book>, takes no parameters.
    */
    
    std::vector<Book>& GetBooks(void);

    /* GetCustomers:
    * returns the Library's customers as a reference to a vector<Customer>, takes no parameters.
    */
    
    std::vector<Customer>& GetCustomers(void);

    /* FindBookByName:
    * returns a Book, takes a const reference to a string as a parameter.
    */
    
    Book FindBookByName(std::string const& book_name);

    /* FindBooksByAuthor:
    * returns a vector of Books, takes a const reference to a string as a parameter.
    */
    
    std::vector<Book> FindBooksByAuthor(std::string const& author_name);

    /* FindAllLoanedBooks():
    * returns a vector of Books, takes no parameters.
    */
    std::vector<Book> FindAllLoanedBooks();

    /* FindCustomer:
    * returns a Customer, takes a const reference to a string representing the Customer's id as a parameter.
    */
    Customer FindCustomer(std::string const& customer_id);

private:
    /* Make variables for:
    * name (string)
    * books (vector<Book>)
    * customers (vector<Customer>)
    */
    std::string name_;
    std::vector<Book> books_;
    std::vector<Customer> customers_;
};

#endif
