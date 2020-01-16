//
// Created by daphnegr@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include "../include/user.h"
#include <list>
#include <map>
#include <string>
#include <Book.h>
#include <algorithm>
#include <iostream>


using namespace std;


user::user(string name, string pass): userName(name),password(pass),books(map<string,vector<Book*>>()), genresByid(map<string,int>()),subid(0), receipt(map<int,string>()), wishlist(vector<string>()) ,books_lock(),genre_lock(),rec_lock(),wish_lock(),ids_lock(), ids(vector<pair<string,int>>()){
    
}


string user::getName() {
    return userName;
}

string user::getBorrowedFrom(string book, string genre){
    std::lock_guard<std::mutex> lock(books_lock);
    if (books.find(genre)!=books.end()){
        for (unsigned int i = 0; i<books.find(genre)->second.size(); i++){
            if (books.find(genre)->second.at(i)->getName() == book){
                return books.find(genre)->second.at(i)->getBorrowFrom();
            }
        }
    }
    return "";
}

void user::changeBorrowedFrom(string book,string genre, string name){
    std::lock_guard<std::mutex> lock(books_lock);
    bool found = false;
    for (unsigned int i = 0; i<books.find(genre)->second.size() && !found; i++){
        if (books.find(genre)->second.at(i)->getName()==book){
            books.find(genre)->second.at(i)->setBorrowFrom(name);
            found = true;
        }
    }
}

vector<pair<string,int>> user::getGenres(){
    std::lock_guard<std::mutex> lock(ids_lock);
    return ids;
}



void user::addbook(string book,string genre) {
    std::lock_guard<std::mutex> lock(books_lock);
    bool found = false;
    if (books.find(genre)==books.end()){
        vector<Book*> *newList = new vector<Book*>;
        books.insert({genre,*newList});
        delete newList;
    }
    else {
        for (unsigned int i = 0; i < books.find(genre)->second.size() && !found; i++) {
            if (books.find(genre)->second.at(i)->getName() == book) {
                found = true;
            }
        }
    }
    if (!found) {
        Book *b = new Book(book, genre);
        books.find(genre)->second.push_back(b);
    }
}

void user::subscribe(string genre,int id) {
    std::lock_guard<std::mutex> lock(genre_lock);
    if (genresByid.find(genre)==genresByid.end()){
        ids.push_back({genre,id});
        genresByid.insert({genre,id});
        if (books.find(genre)==books.end()){
            vector<Book*> *newList = new vector<Book*>;
            books.insert({genre,*newList});
            delete newList;
        }

    }
}


void user::unsubscribe(string genre) {
    std::lock_guard<std::mutex> lock(genre_lock);
    if (genresByid.find(genre)!=genresByid.end()){
        int id = genresByid.find(genre)->second;
        genresByid.erase(genresByid.find(genre));
        vector<pair<string,int>>::iterator it;
        pair<string,int> toRemove = make_pair(genre,id);
        it = find(ids.begin(),ids.end(),toRemove);
        if (it!=ids.end()){
            ids.erase(it);
        }
    }
}


void user::deletebook(string book, string genre) {
    std::lock_guard<std::mutex> lock(books_lock);
    if (books.find(genre)!=books.end()){
        bool found = false;
        for (unsigned int i = 0; i<books.find(genre)->second.size() && !found; i++){
            if(books.find(genre)->second.at(i)->getName()==book){
                books.find(genre)->second.at(i)->hasBook(false);
                found = true;
            }
        }
    }
}

vector<Book*> user::getBooksByGenre(string genre) {
    std::lock_guard<std::mutex> lock(books_lock);
    return books.find(genre)->second;
}

bool user::getbook(string name, string genre){
    std::lock_guard<std::mutex> lock(books_lock);
    if (books.find(genre)!=books.end()){
        for (unsigned int i = 0;i < books.find(genre)->second.size();i++){
            if (books.find(genre)->second.at(i)->getName()==name){
                return true;
            }
        }
    }
    return false;
}

int user::getsubid(){
    subid++;
    return subid;
}

string user::getId(string genre){
    std::lock_guard<std::mutex> lock(genre_lock);
    if (genresByid.find(genre)!=genresByid.end()){
        string id = to_string(genresByid.find(genre)->second);
        return id;
    }
    return "-1";
}

void user::returnedBook(string book,string genre){
    std::lock_guard<std::mutex> lock(books_lock);
    bool found = false;
    for (unsigned int i = 0; i<books.find(genre)->second.size() && !found; i++){
        if (books.find(genre)->second.at(i)->getName()==book){
            books.find(genre)->second.at(i)->hasBook(true);
            found = true;
        }
    }
}

string user::booksByGenre(string genre) {
    std::lock_guard<std::mutex> lock(books_lock);
    string s ="";
    s=s+userName;
    s=s+":";
    if (books.find(genre)->second.size()!=0) {
        for (unsigned int i = 0; i < books.find(genre)->second.size(); i++) {
            if (books.find(genre)->second.at(i)->doesHaveBook()){
                s = s + books.find(genre)->second.at(i)->getName() + ",";
            }
        }
        int last = s.find_last_of(',');
        s = s.substr(0, last);
    }
    return s;
}

user::~user(){
    for (pair<string, vector<Book*>> p: books){
        for (unsigned int i = 0; i<p.second.size();i++){
            delete p.second.at(i);
        }
        p.second.clear();
    }
    ids.clear();
    books.clear();
    genresByid.clear();
    receipt.clear();
}

void user::addReceipt(int id,string action,string genre) {
    std::lock_guard<std::mutex> lock(rec_lock);
    if (action=="join"){
        string ans = "Joined club " + genre;
        receipt.insert({id,ans});
    }
    else if (action =="exit"){
        string ans = "Exited club " + genre;
        receipt.insert({id,ans});
    }
    else if (action=="logout"){
        string ans = "logout";
        receipt.insert({id,ans});
    }
}

string user::getAnswer(int id){
    if (receipt.find(id)!=receipt.end()){
        return receipt.find(id)->second;
    }
    return "-1";
}

void user::logOut() {
    std::lock_guard<std::mutex> lock(books_lock);
    for (pair<string,vector<Book*>> p: books){
        for (unsigned int i = 0; i<p.second.size();i++){
            delete p.second.at(i);
        }
        p.second.clear();
    }
    books.clear();
    genresByid.clear();
    receipt.clear();
}

void user::addToWishList(string book) {
    std::lock_guard<std::mutex> lock(wish_lock);
    bool found = false;
    for (unsigned int i = 0; i<wishlist.size() && !found; i++){
        if (wishlist.at(i)==book){
            found = true;
        }
    }
    if (!found){
        wishlist.push_back(book);
    }
}

bool user::inWishList(string book) {
    std::lock_guard<std::mutex> lock(wish_lock);
    for (unsigned int i = 0; i<wishlist.size();i++){
        if (wishlist.at(i)==book){
            return true;
        }
    }
    return false;
}

void user::removeFromWishList(string book) {
    std::lock_guard<std::mutex> lock(wish_lock);
    vector<string>::iterator it;
    it = find(wishlist.begin(),wishlist.end(),book);
    if (it!=wishlist.end()){
        wishlist.erase(it);
    }
}

void user::changeStatus(string book, string genre) {
    std::lock_guard<std::mutex> lock(books_lock);
    bool found = false;
    for (unsigned int i = 0; i<books.find(genre)->second.size() && !found;i++){
        if (books.find(genre)->second.at(i)->getName()==book){
            books.find(genre)->second.at(i)->hasBook(true);
            found = true;
        }
    }
}

bool user::hasBookInInventory(string book,string genre){
    std::lock_guard<std::mutex> lock(books_lock);
    for (unsigned int i = 0; i<books.find(genre)->second.size(); i++){
        if (books.find(genre)->second.at(i)->getName()==book){
            return books.find(genre)->second.at(i)->doesHaveBook();
        }
    }
    return false;
}

int user::numOfBooksTaken(string genre) {
    std::lock_guard<std::mutex> lock(books_lock);
    int num = 0;
    for (unsigned int i = 0; i<books.find(genre)->second.size(); i++){
        if (books.find(genre)->second.at(i)->doesHaveBook()){
            num++;
        }
    }
    return num;
}
