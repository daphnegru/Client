//
// Created by noambob@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_MAINCLIENT_H
#define BOOST_ECHO_CLIENT_MAINCLIENT_H

#include <string>
#include <vector>
using namespace std;

class mainclient {
public:
    mainclient();
    vector<string> splitlinefromserver(string line);
    vector<string> splitline(string line);
    vector<string> splitCommand(string line);
};


#endif //BOOST_ECHO_CLIENT_MAINCLIENT_H