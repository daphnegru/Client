//
// Created by noambob@wincs.cs.bgu.ac.il on 10/01/2020.
//
#include "connectionHandler.h"
#include "user.h"

#ifndef BOOST_ECHO_CLIENT_READSOCKET_H
#define BOOST_ECHO_CLIENT_READSOCKET_H
using namespace std;


class readsocket {
public:
    void run();
    vector<string> splitlines(string line);
    vector<string> splittowords(string line);
    readsocket(ConnectionHandler &connectionHandler,user u);


private:
    ConnectionHandler &handle;
    user u;
};


#endif //BOOST_ECHO_CLIENT_READSOCKET_H