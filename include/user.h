//
// Created by daphnegr@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#include <string>
#include <list>
#include <map>
#include <vector>
#include "Book.h"

using namespace std;

class user {
public:
    user(string userName, string password);
    void addbook(string book,string genre);
    void deletebook(string book, string genre);
    void subscribe(string genre, int id);
    void unsubscribe(string genre);
    map<string,vector<Book*>> getBooks();
    vector<Book*> getBooksByGenre(string genre);
    string getName();
    string getBorrowedFrom(string book, string genre);
    bool getbook(string name, string genre);
    void borrowbookToSomeone(string book, string genre);
    int getsubid();
    void returnedBook(string book,string genre);
    string booksByGenre(string genre);

private:
    map<string,vector<Book*>> books;
    string userName;
    string password;
//    bool login;
    map<string,int> genresByid;
    int subid;


};


#endif //BOOST_ECHO_CLIENT_USER_H
