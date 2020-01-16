//
// Created by daphnegr@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#include <string>
#include <list>
#include <map>
#include <vector>
#include <mutex>
#include "Book.h"

using namespace std;

class user {
public:
    user(string userName, string password);
    ~user();
    void addbook(string book,string genre);
    void deletebook(string book, string genre);
    void subscribe(string genre, int id);
    void unsubscribe(string genre);
    map<string,vector<Book*>> getBooks();
    vector<Book*> getBooksByGenre(string genre);
    string getName();
    string getBorrowedFrom(string book, string genre);
    bool getbook(string name, string genre);
    int getsubid();
    void returnedBook(string book,string genre);
    string booksByGenre(string genre);
    string getId(string genre);
    string getAnswer(int id);
    void addReceipt(int id,string action,string genre);
    void logOut();
    void addToWishList(string book);
    bool inWishList(string book);
    void removeFromWishList(string book);
    void changeStatus(string book, string genre);
    void changeBorrowedFrom(string book,string genre, string name);
    bool hasBookInInventory(string book, string genre);
    int numOfBooksTaken(string genre);
    vector<pair<string,int>> getGenres();


private:
    string userName;
    string password;
    map<string,vector<Book*>> books;
    map<string,int> genresByid;
    int subid;
    map<int,string>receipt;
    vector<string> wishlist;
    std::mutex books_lock;
    std::mutex genre_lock;
    std::mutex rec_lock;
    std::mutex wish_lock;
    std::mutex ids_lock;
    vector<pair<string,int>> ids;




};


#endif //BOOST_ECHO_CLIENT_USER_H
