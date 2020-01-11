//
// Created by noambob@wincs.cs.bgu.ac.il on 09/01/2020.
//
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "mainclient.h"
#include "connectionHandler.h"
#include <thread>
#include<boost/thread.hpp>
#include "keyboard.h"
#include "readsocket.h"
#include "user.h"
vector<string> splitline(string basicString);

using namespace std;
mainclient::mainclient() {
}

int main (int argc, char *argv[]) {
    bool login = false;
    while (!login) {
        string line = "";
        getline(cin, line);
        vector<string> split = splitline(line);
        if (split.at(0) == "login") {
            string socket= split.at(1);
            string name= split.at(2);
            string pass= split.at(3);
            string host=socket.substr(0,line.find_first_of(':'));
            string port=socket.substr(line.find_first_of(':')+1);
            short portshort=stoul(port);
            ConnectionHandler *handler= new ConnectionHandler(host,portshort);
            bool con= handler->connect();
            login=con;
            if(login) {
                string connect = string("CONNECT") + "\n" + string("accept-version:1.2") + "\n" +
                                 string("host:stomp.cs.bgu.ac.il") + "\n" + string("login:") + name + "\n" +
                                 string("passcode:") + pass + "\n" + "\0";
                handler->sendLine(connect);
                string str = "";
                handler->getLine(str);
                vector<string> split = splitline(str);
                bool login = false;
                if (split.at(0) == "CONNECTED") {
                    user *u = new user(name, pass);
                    login = true;
                    keyboard *k = new keyboard(*handler, *u);
                    readsocket *r = new readsocket(*handler,*u);
                    thread key(&keyboard::run,k);
                    thread socket(&readsocket::run, r);
//                    boost::thread key(k);
//                    boost::thread socket(r);
                }
            }





        }
    }

}

vector<string> splitline(string line) {
    vector<string> *linesplit= new vector<string>; // delete
    while (line.size() != 0) {
        string word = line.substr(0, line.find_first_of(' '));
        linesplit->push_back(word);
        line.substr(0, line.find_first_of(' '));
    }
    return *linesplit;
}

//vector<string> readsocket::splitlinesfromserver(string line) {
//    vector<string> *linesplit= new vector<string>; // delete
//    while (line!="\0") {
//        string word = line.substr(0, line.find_first_of('\n'));
//        linesplit->push_back(word);
//        line.substr(0, line.find_first_of('\n'));
//    }
//    return *linesplit;
//}
vector<string> connectuser(string name,string pass){
    vector<string> *user= new vector<string>;
    user->push_back(pass);
    user->push_back(name);
    return *user;
}