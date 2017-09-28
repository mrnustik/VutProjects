//
// Created by Michal Mrnustik on 26/09/2017.
//

#ifndef ISA_ARGUMENTS_H
#define ISA_ARGUMENTS_H

#include <string>
#include <iostream>
#include "Logger.h"

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
    static Arguments* ParseArguments(int argc, char* argv[]);
	static Arguments* GetInstance();
private:
	static Arguments* instance;
    std::string networkString;
};


#endif //ISA_ARGUMENTS_H
