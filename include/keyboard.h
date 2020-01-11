//
// Created by noambob@wincs.cs.bgu.ac.il on 10/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_KEYBOARD_H
#define BOOST_ECHO_CLIENT_KEYBOARD_H

#include "connectionHandler.h"
#include <vector>
#include <string>
#include "user.h"

using namespace std;

class keyboard{
public:
    keyboard(ConnectionHandler &handler, user u);
    void run();
    vector<string> splitline(string line);

private:
    ConnectionHandler &handle;
    user u;
};


#endif //BOOST_ECHO_CLIENT_KEYBOARD_H