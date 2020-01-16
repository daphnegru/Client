//
// Created by noambob@wincs.cs.bgu.ac.il on 09/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_MAINCLIENT_H
#define BOOST_ECHO_CLIENT_MAINCLIENT_H

#include <string>
#include <vector>
#include <mutex>

using namespace std;

class mainclient {

public:
    ~mainclient();
    mainclient();
    vector<string> splitlinefromserver(string line);
    vector<string> splitline(string line,vector<string> &linesplit);
    vector<string> splitCommand(string line,vector<string> &linesplit);



};


#endif //BOOST_ECHO_CLIENT_MAINCLIENT_H