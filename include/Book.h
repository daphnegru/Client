//
// Created by daphnegr@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOK_H
#define BOOST_ECHO_CLIENT_BOOK_H
using namespace std;

#include <string>

class Book{
public:
    Book(string name,string genre);
    string getName();
    string getGenre();
    string getBorrowFrom();
    bool doesHaveBook();
    void setBorrowFrom(string name);
    void hasBook(bool stat);


private:
    string name;
    string genre;
    string borrowedFrom;
    bool haveBook;

};



#endif //BOOST_ECHO_CLIENT_BOOK_H
