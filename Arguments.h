//
// Created by Michal Mrnustik on 26/09/2017.
//

#ifndef ISA_ARGUMENTS_H
#define ISA_ARGUMENTS_H

#include <string>
#include <iostream>

class Arguments {
public:
    Arguments();
    bool flagHelp;
    std::string interfaceName;
    bool flagTcp;
    bool flagUdp;
    int portNumber;
    int maxRtt;
    static const int AllPorts = -1;
    static Arguments* parseArguments(int argc, char* argv[]);
private:
    std::string networkString;
};


#endif //ISA_ARGUMENTS_H
