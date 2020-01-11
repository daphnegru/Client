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
    string str = "";
    handle.getLine(str);
    cout<< str<< endl;
    vector<string> split = splitlines(str);
    bool login=true;
    while (login) {
        if (split.at(0) == "RECEIPT") {
            string rec= split.at(1).substr(':');
            int num= stoi(rec);
            if(num%2==0){
                login=false;
            }

        }
        if (split.at(0) == "MESSAGE") {
            if(split.at(4).find("has added ")){

            }

            if(split.at(4).find("wish")){
                vector<string> words=splittowords(split.at(4));
                string genre= split.at(3).substr(':');
                if(u.getbook(words.at(4),genre)){
                    string message = string("SEND")+"\n"+u.getName()+"\n"+ string("has")+words.at(4)+"\n"+"\0";
                    handle.sendLine(message);
                }

            }

            if(split.at(4).find(" has")){
                vector<string> words= splittowords(split.at(4));
                string genre= split.at(3).substr(':');
                string message=string("SEND")+"\n"+"destination: "+genre+"\n"+"Taking"+words.at(4)+" from "+u.getName()+"\n"+"\0";

            }
            if(split.at(4).find("Taking")){
                vector<string> words= splittowords(split.at(4));
                string genre= split.at(3).substr(':');
                u.addbook(words.at(1),genre);
            }
            if(split.at(4).find("Returning")){
                vector<string> words= splittowords(split.at(4));
                string genre= split.at(3).substr(':');
                if(words.at(3)==u.getName()){
                    u.returnedBook(words.at(1),genre);
                }
            }
            if(split.at(4).find("Book status")){
                string genre= split.at(3).substr(':');
                string s=u.booksByGenre(genre);
                string message= string("SEND")+"\n"+string("destination:")+genre+"\n"+ u.getName()+string(":")+s+"\n"+"\0";
                handle.sendLine(message);

            }
            if(split.at(4).find(":")) {

            }

        }



    }
}

vector<string> readsocket::splitlines(string line) {
    vector<string> *linesplit= new vector<string>; // delete
    while (line!="\0") {
        string word = line.substr(0, line.find_first_of('\n'));
        linesplit->push_back(word);
        line.substr(0, line.find_first_of('\n'));
    }
    return *linesplit;
}
vector<string> readsocket::splittowords(string line) {
    vector<string> *linesplit = new vector<string>; // delete
    while (line.size() != 0) {
        string word = line.substr(0, line.find_first_of(' '));
        linesplit->push_back(word);
        line.substr(0, line.find_first_of(' '));
    }
    return *linesplit;
}
