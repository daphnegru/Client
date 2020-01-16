//
// Created by noambob@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <connectionHandler.h>
#include "readsocket.h"
#include "user.h"

using namespace std;

readsocket::readsocket(ConnectionHandler &_handler, user &use) : handle(_handler), u(use) {

}

void readsocket::run() {
    bool login = true;
    while (login) {
        string str = "";
        handle.getLine(str);
        if (str.size() != 0) {
            vector<string> split;
            split = splitlines(str, split);
            if (split.at(0) == "RECEIPT") {
                string rec = split.at(1).substr(split.at(1).find_first_of(':') + 1);
                int num = stoi(rec);
                string ans = u.getAnswer(num);
                if (ans!="-1") {
                    vector<string> action;
                    action = splittowords(ans, action);
                    if (action.at(0) == "Joined") {
                        string genre = "";
                        for (unsigned int i = 2; i < action.size(); i++) {
                            genre = action.at(i) + " ";
                        }
                        genre = genre.substr(0, genre.find_last_of(' '));
                        u.subscribe(genre, num);
                        vector<string> list;
                        cout << genre + ": " + ans << endl;
                    }
                    if (action.at(0) == "Exited") {
                        cout<<str<<endl;
                        vector<string> list;
                        string genre = "";
                        for (unsigned int i = 2; i < action.size(); i++) {
                            genre = action.at(i) + " ";
                        }
                        genre = genre.substr(0, genre.find_last_of(' '));
                        u.unsubscribe(genre);
                        cout << genre + ": " + ans << endl;
                    }
                    if (action.at(0) == "logout") {
                        u.logOut();
                        login = false;
                    }
                }
            }

            if (split.at(0) == "MESSAGE") {
                string print = split.at(3).substr(split.at(3).find_first_of(':') + 1) + ": " + split.at(5);
                cout << print << endl;
                vector<string> words;
                words = splittowords(split.at(5), words);
                if (findDots(words.at(0))) {
                    string s = "";
                    s = s + words.at(0);
                    for (unsigned int i = 1; i < words.size(); i++) {
                        s = s + " " + words.at(i);
                    }
                }
                else {
                    if (words.size() == 2) {
                        if (words.at(1) == "status") {
                            string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                            if (u.getId(genre) != "-1") {
                                if (u.getBooksByGenre(genre).size() != 0) {
                                    if (u.numOfBooksTaken(genre) != 0) {
                                        string s = u.booksByGenre(genre);
                                        string message =
                                                string("SEND") + '\n' + string("destination:") + genre + '\n' + '\n' +
                                                s +
                                                '\n';
                                        handle.sendLine(message);
                                    }
                                }
                            }
                        }
                    }

                    if (words.size() > 4) {
                        if (words.at(2) == "added") {
                            string bookName = "";
                            for (unsigned int l = 5; l < words.size(); l++) {
                                bookName = bookName + words.at(l) + " ";
                            }
                            if (u.inWishList(bookName)){
                                u.removeFromWishList(bookName);
                            }
                            bookName = bookName.substr(0, bookName.find_last_of(' '));
                            string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                        }
                        if (words.at(1) == "wish") {
                            string bookName = "";
                            for (unsigned int l = 4; l < words.size(); l++) {
                                bookName = bookName + words.at(l) + " ";
                            }
                            bookName = bookName.substr(0, bookName.find_last_of(' '));
                            string userName = words.at(0);
                            string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                            if (u.getbook(bookName, genre) && u.hasBookInInventory(bookName, genre)) {
                                string message =
                                        string("SEND") + '\n' + "destination:" + genre + '\n' + '\n' + u.getName() +
                                        string(" has ") + bookName + '\n';
                                handle.sendLine(message);
                            }
                        }
                    }

                    if (words.at(1) == "has" && words.at(2) != "added") {
                        string bookName = "";
                        for (unsigned int l = 2; l < words.size(); l++) {
                            bookName = bookName + words.at(l) + " ";
                        }
                        bookName = bookName.substr(0, bookName.find_last_of(' '));
                        string bookFrom = words.at(0);
                        if (u.inWishList(bookName)) {
                            string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                            string message = string("SEND") + '\n' + "destination:" + genre + '\n' + '\n' + "Taking " +
                                             bookName + " from " + bookFrom + '\n';
                            handle.sendLine(message);
                        }

                    }
                    if (words.at(0) == "Taking") {
                        string userName = words.at(words.size() - 1);
                        string bookName = "";
                        bool found = false;
                        for (unsigned int i = 1; i < words.size()-2 && !found; i++) {
                            bookName = bookName + words.at(i) + " ";
                        }
                        bookName = bookName.substr(0, bookName.find_last_of(' '));
                        string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                        if (u.getName() == userName && u.hasBookInInventory(bookName, genre)) {
                            u.deletebook(bookName, genre);
                        }
                        if (u.inWishList(bookName)) {
                            u.addbook(bookName, genre);
                            u.changeStatus(bookName, genre);
                            u.changeBorrowedFrom(bookName, genre, userName);
                            u.removeFromWishList(bookName);
                        }
                    }

                    if (words.at(0) == "Returning") {
                        string bookName = "";
                        bool found = false;
                        for (unsigned int i = 1; i < words.size()-2 && !found; i++) {
                            bookName = bookName + words.at(i) + " ";
                        }
                        bookName = bookName.substr(0, bookName.find_last_of(' '));
                        string toReturn = words.at(words.size() - 1);
                        string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                        if (toReturn == u.getName() && !u.hasBookInInventory(bookName, genre) ) {
                            u.returnedBook(bookName, genre);
                        }
                    }
                }
            }
        }
    }
}

bool readsocket::findDots(string line) {
    for (unsigned int i = 0; i < line.size(); i++) {
        if (line.at(i) == ':') {
            return true;
        }
    }
    return false;
}

vector<string> readsocket::splitlines(string line, vector<string> &linesplit) {

    while (line.size() != 0) {
        int first = line.find_first_of('\n');
        string word = line.substr(0, first);
        linesplit.push_back(word);
        first = line.find_first_of('\n');
        if (first != -1) {
            line = line.substr(first + 1);
        } else {
            line = "";
        }
    }
    return linesplit;
}


vector<string> readsocket::splittowords(string line, vector<string> &linesplit) {

    while (line.size() != 0) {
        int s = line.find_first_of(' ');
        string word = line.substr(0, s);
        linesplit.push_back(word);
        s = line.find_first_of(' ');
        if (s != -1) {
            line = line.substr(s + 1);
        } else {
            line = "";
        }
    }
    return linesplit;
}
