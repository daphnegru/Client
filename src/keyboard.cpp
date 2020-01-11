//
// Created by noambob@wincs.cs.bgu.ac.il on 10/01/2020.
//
#include <connectionHandler.h>
#include "readsocket.h"
#include "user.h"
#include <keyboard.h>

using namespace std;

keyboard::keyboard(ConnectionHandler &_handler,user use) : handle(_handler),u(use) {}


void keyboard::run() {
//    string str = "";
//    getline(cin, str);
//    vector<string> split = splitline(str);
    int recjoin=1;
    int reclogout=2;
    bool login = true;
    while (login) {
        string str = "";
        getline(cin, str);
        vector<string> split = splitline(str);
        if (split.at(0) == "join") {
            string sub= to_string(u.getsubid());
            string message=string("SUBSCRIBE")+'\n'+string("destination:")+split.at(1)+'\n'+string("id:")+sub+'\n'+string("receipt:")+to_string(recjoin)+'\n'+'\0';
            recjoin=recjoin+2;
            u.subscribe(split.at(1),stoi(sub));
            handle.sendLine(message);
            string str="Joined club "+split.at(1);
            cout<<str<< endl;
        }
        if(split.at(0)=="exit"){
            string sub = u.getId(split.at(1));
            string message=string("UNSUBSCRIBE")+'\n'+string("destination:")+split.at(1)+'\n'+string("id:")+sub+'\n'+string("receipt:")+to_string(recjoin)+'\n'+'\0';
            recjoin=recjoin+2;
            u.unsubscribe(split.at(1));
            handle.sendLine(message);
            string str="Exited club "+split.at(1);
            cout<<str<< endl;
        }
        if (split.at(0) == "add") {
            string bookName = "";
            for (int i = 2; i<split.size();i++){
                bookName=bookName+split.at(i)+" ";
            }
            bookName=bookName.substr(0,bookName.find_last_of(' '));
            string message=string("SEND")+'\n'+string("destination:")+split.at(1)+'\n'+u.getName()+string(" has added the book ")+bookName+'\n'+'\0';
            u.addbook(split.at(2),split.at(1));
            handle.sendLine(message);
            string str = u.getName() + string(" ") + string("has added the book ") + split.at(2);
//            cout <<str<< endl;
        }

        if(split.at(0) == "borrow"){
            string bookName = "";
            for (int i = 2; i<split.size();i++){
                bookName=bookName+split.at(i)+" ";
            }
            bookName=bookName.substr(0,bookName.find_last_of(' '));
            string message=string("SEND")+'\n'+string("destination:")+split.at(1)+'\n'+u.getName()+string(" wish to borrow ")+bookName+'\n'+'\0';
            handle.sendLine(message);
        }
        if(split.at(0) == "return"){
            string bookName = "";
            for (int i = 2; i<split.size();i++){
                bookName=bookName+split.at(i)+" ";
            }
            bookName=bookName.substr(0,bookName.find_last_of(' '));
            string message=string("SEND")+'\n'+string("destination:")+split.at(1)+'\n'+string(" Returning ")+bookName+string("to ")+u.getBorrowedFrom(split.at(1),split.at(2))+'\n'+'\0';
            u.deletebook(split.at(2),split.at(1));
            handle.sendLine(message);
        }
        if(split.at(0) == "status"){
            string message=string("SEND")+'\n'+string("destination:")+split.at(1)+'\n'+string("book status")+'\n'+'\0';
            handle.sendLine(message);
        }
        if(split.at(0) == "logout"){
            string message=string("DISCONNECT")+'\n'+string("receipt:")+to_string(reclogout)+'\n'+'\0';
            reclogout=reclogout+2;
            handle.sendLine(message);
            login=false;
        }
    }
}

vector<string> keyboard::splitline(string line) {
    vector<string> *linesplit= new vector<string>; // delete
    while (line.size() != 0) {
        string word = line.substr(0, line.find_first_of(' '));
        linesplit->push_back(word);
        int space = line.find_first_of(' ');
        if (space !=-1){
            line=line.substr(space+1);
        }
        else {
            line = "";
        }
    }
    return *linesplit;
}

