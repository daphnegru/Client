//
// Created by noambob@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_KEYBOARD_H
#define BOOST_ECHO_CLIENT_KEYBOARD_H

#include "connectionHandler.h"
#include <vector>
#include <string>
#include <mutex>
#include "user.h"

using namespace std;

class keyboard{
public:
    keyboard(ConnectionHandler &handler, user &u);
    void run();
    vector<string> splitline(string line, vector<string> &linesplit);

private:
    ConnectionHandler &handle;
    user &u;
    int id;


};


#endif //BOOST_ECHO_CLIENT_KEYBOARD_H