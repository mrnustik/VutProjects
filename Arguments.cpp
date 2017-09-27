//
// Created by Michal Mrnustik on 26/09/2017.
//

#include "Arguments.h"

Arguments *Arguments::parseArguments(int argc, char **argv) {
    auto *arguments = new Arguments;
    for (int i = 0; i < argc; ++i) {
        std::string currentArg(argv[i]);
        if(currentArg == "-h" || currentArg == "--help"){
            arguments->flagHelp = true;
        } else if(currentArg == "-t"){
            arguments->flagTcp = true;
        } else if(currentArg == "-u"){
            arguments->flagUdp = true;
        } else if(currentArg == "-p" || currentArg == "--port") {
            if (++i >= argc) {
                std::cerr << "Missing port number";
                return nullptr;
            }
            std::string port(argv[i]);
            try {
                arguments->portNumber = stoi(port);
            } catch (...) {
                std::cerr << "Could not parse port number." << argv[i];
                return nullptr;
            }
            if (arguments->portNumber < 0 || arguments->portNumber > 65535) {
                std::cerr << "Invalid port number: " << arguments->portNumber;
                return nullptr;
            }
        } else if(currentArg == "-w" || currentArg == "--wait") {
            if (++i >= argc) {
                std::cerr << "Missing maximum RTT";
                return nullptr;
            }
            std::string wait(argv[i]);
            try {
                arguments->maxRtt = stoi(wait);
            } catch (...) {
                std::cerr << "Could not parse maximum RTT: " << argv[i];
                return nullptr;
            }
            if (arguments->maxRtt < 0) {
                std::cerr << "Invalid maximum RTT: " << arguments->portNumber;
                return nullptr;
            }
        } else if(currentArg == "-n" || currentArg == "--network"){
            if(++i >= argc) {
                std::cerr << "Missing network defininiton";
                return nullptr;
            }
        } else {
            std::cerr << "Unexpected argument: " << argv[i];
            return nullptr;
        }

    }
    return arguments;
}

Arguments::Arguments() {
    this->flagHelp = false;
    this->flagTcp = false;
    this->flagUdp = false;
    this->networkString = "";
    this->interfaceName = "";
    this->maxRtt = 1000;
    this->portNumber = AllPorts;
}
