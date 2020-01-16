//
// Created by noambob@wincs.cs.bgu.ac.il on 10/01/2020.
//
#include <connectionHandler.h>
#include "readsocket.h"
#include "user.h"
#include <keyboard.h>

using namespace std;

keyboard::keyboard(ConnectionHandler &_handler, user &use) : handle(_handler), u(use), id(0) {

}


void keyboard::run() {
    bool login = true;
    while (login) {
        string str = "";
        getline(cin, str);
        vector<string> split;
        split = splitline(str, split);
        if (split.at(0) == "join") {
            string sub = to_string(u.getsubid());
            id++;
            string message =
                    string("SUBSCRIBE") + '\n' + string("destination:") + split.at(1) + '\n' + string("id:") + sub +
                    '\n' + string("receipt:") + to_string(id) + '\n' + '\n';
            u.addReceipt(id, "join", split.at(1));
            handle.sendLine(message);
        }
        if (split.at(0) == "exit") {
            string sub = u.getId(split.at(1));
            if (sub != "-1") {
                id++;
                string message =
                        string("UNSUBSCRIBE") + '\n' + string("id:") + sub + '\n' + string("receipt:") + to_string(id) +
                        '\n' + '\n';
                u.addReceipt(id, "exit", split.at(1));
                handle.sendLine(message);
            }

        }
        if (split.at(0) == "add") {
            string bookName = "";
            for (unsigned int i = 2; i < split.size(); i++) {
                bookName = bookName + split.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.find_last_of(' '));
            string message = string("SEND") + '\n' + string("destination:") + split.at(1) + '\n' + '\n' + u.getName() +
                             string(" has added the book ") + bookName + '\n';
            u.addbook(bookName, split.at(1));
            handle.sendLine(message);
        }
        if (split.at(0) == "borrow") {
            string bookName = "";
            for (unsigned int i = 2; i < split.size(); i++) {
                bookName = bookName + split.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.find_last_of(' '));
            u.addToWishList(bookName);
            string message = string("SEND") + '\n' + string("destination:") + split.at(1) + '\n' + '\n' + u.getName() +
                             string(" wish to borrow ") + bookName + '\n';
            handle.sendLine(message);
        }
        if (split.at(0) == "return") {
            string bookName = "";
            for (unsigned int i = 2; i < split.size(); i++) {
                bookName = bookName + split.at(i) + " ";
            }
            bookName = bookName.substr(0, bookName.find_last_of(' '));
            string takeFrom = u.getName();
            string toReturn = u.getBorrowedFrom(bookName, split.at(1));
            if (toReturn != "") {
                u.deletebook(bookName, split.at(1));
                string message =
                        string("SEND") + '\n' + string("destination:") + split.at(1) + '\n' + '\n' +
                        string("Returning ") +
                        bookName + string(" to ") + toReturn + '\n';
                handle.sendLine(message);
            }
        }
        if (split.at(0) == "status") {
            string message =
                    string("SEND") + '\n' + string("destination:") + split.at(1) + '\n' + '\n' + string("book status") +
                    '\n';
            handle.sendLine(message);
        }
        if (split.at(0) == "logout") {
            id++;
            string message = string("DISCONNECT") + '\n' + string("receipt:") + to_string(id) + '\n' + '\n';
            u.addReceipt(id, "logout", "");
            handle.sendLine(message);
            login = false;
        }
    }
}

vector<string> keyboard::splitline(string line, vector<string> &linesplit) {

    while (line.size() != 0) {
        string word = line.substr(0, line.find_first_of(' '));
        linesplit.push_back(word);
        int space = line.find_first_of(' ');
        if (space != -1) {
            line = line.substr(space + 1);
        } else {
            line = "";
        }
    }
    return linesplit;
}

