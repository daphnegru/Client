//
// Created by daphnegr@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <string>
#include <Book.h>

using namespace std;

Book::Book(string _name, string _genre): name(_name), genre(_genre),borrowedFrom(""), haveBook(true) {

}


string Book::getBorrowFrom() {
    return borrowedFrom;
}

string Book::getGenre() {
    return genre;
}

string Book::getName() {
    return name;
}

bool Book::doesHaveBook(){
    return haveBook;
}

void Book::setBorrowFrom(string name) {
    borrowedFrom=name;
}

void Book::hasBook(bool stat){
    haveBook=stat;
}