//
// Created by daphnegr@wincs.cs.bgu.ac.il on 09/01/2020.
//

#include "../include/user.h"
#include <list>
#include <map>
#include <string>
#include <Book.h>


using namespace std;


user::user(string name, string pass): userName(name),password(pass) {
    map<string,vector<Book*>> books;
    map<string,int> genresByid;
    subid=0;

}

string user::getName() {
    return userName;
}

string user::getBorrowedFrom(string book, string genre){
    if (books.find(genre)!=books.end()){
        for (int i = 0; i<books.find(genre)->second.size(); i++){
            if (books.find(genre)->second.at(i)->getName() == book){
                return books.find(genre)->second.at(i)->getBorrowFrom();
            }
        }
    }
}

void user::borrowbookToSomeone(string book, string genre){
    if (books.find(genre)!=books.end()){
        bool found = false;
        for (int i = 0; i<books.find(genre)->second.size() && !found; i++){
            if (books.find(genre)->second.at(i)->getName()==book){
                books.find(genre)->second.at(i)->hasBook(false);
                found = true;
            }
        }
    }
}

void user::addbook(string book,string genre) {
    bool found = false;
    if (books.find(genre)==books.end()){
        vector<Book*> *newList = new vector<Book*>;
        books.insert({genre,*newList});
    }
    else {
        for (int i = 0; i < books.find(genre)->second.size() && !found; i++) {
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
    if (genresByid.find(genre)==genresByid.end()){
        genresByid.insert({genre,id});
    }
}

void user::unsubscribe(string genre) {
    if (genresByid.find(genre)!=genresByid.end()){
        genresByid.erase(genresByid.find(genre));
    }
}



map<string,vector<Book*>> user::getBooks(){
    return books;
}

void user::deletebook(string book, string genre) {
    if (books.find(genre)!=books.end()){
        bool found = false;
        for (int i = 0; i<books.find(genre)->second.size() && !found; i++){
            if (books.find(genre)->second.at(i)->getName()==book){
                if (books.find(genre)->second.at(i)->doesHaveBook()){
                    vector<Book*> *newList = new vector<Book*>;
                    for (int j = 0; i<books.find(genre)->second.size();j++){
                        if (books.find(genre)->second.at(j)->getName()!=book){
                            newList->push_back(books.find(genre)->second.at(j));
                        }
                    }
                    books.find(genre)->second.at(i)->hasBook(false);
                    books.find(genre)->second = *newList;
                    newList->clear();
                    found = true;
                }
            }
        }
    }
}

vector<Book*> user::getBooksByGenre(string genre) {
    if (books.find(genre)!=books.end()){
        return books.find(genre)->second;
    }
}

bool user::getbook(string name, string genre){
    if (books.find(genre)!=books.end()){
        for (int i = 0; books.find(genre)->second.size();i++){
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
    if (genresByid.find(genre)!=genresByid.end()){
        string id = to_string(genresByid.find(genre)->second);
        return id;
    }
    return nullptr;
}

void user::returnedBook(string book,string genre){
    bool found = false;
    for (int i = 0; i<books.find(genre)->second.size() && !found; i++){
        if (books.find(genre)->second.at(i)->getName()==book){
            books.find(genre)->second.at(i)->hasBook(true);
            found = true;
        }
    }
}

string user::booksByGenre(string genre) {
    string s ="";
    s=s+userName;
    s=s+":";
    for (int i = 0; i<getBooksByGenre(genre).size();i++){
        s=s+getBooksByGenre(genre).at(i)->getName()+",";
    }
    int last = s.find_last_of(',');
    s=s.substr(0,last);
    return s;
}

user::~user(){
    for (pair<string, vector<Book*>> p: books){
        for (int i = 0; i<p.second.size();i++){
            delete p.second.at(i);
        }
        p.second.clear();
    }
    books.clear();
    genresByid.clear();
}

