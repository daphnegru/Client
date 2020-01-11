//
// Created by noambob@wincs.cs.bgu.ac.il on 10/01/2020.
//

#include <connectionHandler.h>
#include "readsocket.h"
#include "user.h"
using namespace std;

readsocket::readsocket(ConnectionHandler &_handler,user user) : handle(_handler),u(user){

}

void readsocket::run() {
//    string str = "";
//    handle.getLine(str);
//    cout<< str<< endl;
//    vector<string> split = splitlines(str);
    bool login=true;
    while (login) {
        string str = "";
        handle.getLine(str);
        cout<< str<< endl;
        if (str.size()!=0) {
            vector<string> split = splitlines(str);
            if (split.at(0) == "RECEIPT") {
                string rec = split.at(1).substr(':');
                int num = stoi(rec);
                if (num % 2 == 0) {
                    login = false;
                }

            }
            if (split.at(0) == "MESSAGE") {
//                int space = split.at(4).find_first_of(':');
//                if (space == -1) {
//                    //what to do with the Book status
////                    string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
////                    string status = u.booksByGenre(genre);
////
////                    handle.sendLine(status);
//                }
//                else {
                    vector<string> words = splittowords(split.at(4));
                    if (words.size() == 2) {
                        if (words.at(1) == "status") {
                            string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                            string s = u.booksByGenre(genre);
                            string message =
                                    string("SEND") + '\n' + string("destination:") + genre + '\n' + s + '\n' + '\0';
                            handle.sendLine(message);
                        }
                    }

                    if (words.size() > 4) {
                        if (words.at(2) == "added") {
                            string bookName = "";
                            for (int l = 5; l < words.size(); l++) {
                                bookName = bookName + words.at(l) + " ";
                            }
                            bookName = bookName.substr(0, bookName.find_last_of(' '));
                            string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                            u.addbook(bookName, genre);
                        }
                        if (words.at(3) == "wish") {
                            string bookName = "";
                            for (int l = 5; l < words.size(); l++) {
                                bookName = bookName + words.at(l) + " ";
                            }
                            bookName = bookName.substr(0, bookName.find_last_of(' '));
                            string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                            if (u.getbook(words.at(4), genre)) {
                                string message =
                                        string("SEND") + '\n' + u.getName() + '\n' + string("has") + words.at(4) +
                                        '\n' + '\0';
                                handle.sendLine(message);
                            }

                        }
                    }

                    if (words.at(1) == "has" && words.at(2) != "added") {
                        string bookName = "";
                        for (int l = 2; l < words.size(); l++) {
                            bookName = bookName + words.at(l) + " ";
                        }
                        bookName = bookName.substr(0, bookName.find_last_of(' '));
                        string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                        string message =
                                string("SEND") + '\n' + "destination: " + genre + '\n' + "Taking" + words.at(4) +
                                " from " +
                                u.getName() + '\n' + '\0';
                    }

                    if (words.at(0) == "Taking") {
                        string bookName = "";
                        bool found = false;
                        for (int i = 1; i < words.size() && !found; i++) {
                            if (words.at(i) != "from") {
                                bookName = bookName + words.at(i) + " ";
                            } else {
                                found = true;
                            }
                        }
                        bookName = bookName.substr(0, bookName.find_last_of(' '));
                        string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                        u.addbook(words.at(1), genre);
                    }
                    if (words.at(0) == "Returning") {
                        string bookName = "";
                        bool found = false;
                        for (int i = 1; i < words.size() && !found; i++) {
                            if (words.at(i) != "to") {
                                bookName = bookName + words.at(i) + " ";
                            } else {
                                found = true;
                            }
                        }
                        bookName = bookName.substr(0, bookName.find_last_of(' '));
                        string genre = split.at(3).substr(split.at(3).find_first_of(':') + 1);
                        if (words.at(3) == u.getName()) {
                            u.returnedBook(words.at(1), genre);
                        }
                    }
                    if (words.size()==1 || findDots(words.at(0))){
                        //what to do with frame recived after send status
                        string s="";
                        s=s+words.at(0);
                        for (int i = 1; i<words.size();i++){
                            s=s+" "+words.at(i);
                        }
                    }
                }
//            }
        }

    }
}

bool readsocket::findDots(string line){
    for (int i = 0; i<line.size();i++){
        if (line.at(i)==':'){
            return true;
        }
    }
    return false;
}

vector<string> readsocket::splitlines(string line) {
    vector<string> *linesplit= new vector<string>; // delete
    while (line.size() != 0) {
        int last = line.find_last_of('\n');
        int space = line.find_first_of(' ');
        if (last<space){
            int first = line.find_first_of('\n');
            string word = line.substr(0,first);
            linesplit->push_back(word);
            first = line.find_first_of('\n');
            if (first!=-1){
                line = line.substr(first +1);
            }
            else {
                int n = line.find_first_of('\0');

                line = "";
            }
        }
        else {
            int s = line.find_first_of(' ');
            string word = line.substr(0, s);
            linesplit->push_back(word);
            s = line.find_first_of(' ');
            if (s != -1) {
                line = line.substr(s + 1);
            } else {
                line = "";
            }
        }
    }
    return *linesplit;
}
vector<string> readsocket::splittowords(string line) {
    vector<string> *linesplit = new vector<string>; // delete
    while (line.size() != 0) {
        int s = line.find_first_of(' ');
        string word = line.substr(0, s);
        linesplit->push_back(word);
        s = line.find_first_of(' ');
        if (s != -1) {
            line = line.substr(s + 1);
        } else {
            line = "";
        }
    }
    return *linesplit;
}
