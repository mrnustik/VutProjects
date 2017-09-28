//
// Created by Michal Mrnustik on 26/09/2017.
//

#include "Arguments.h"

Arguments *Arguments::ParseArguments(int argc, char *argv[]) {
    auto *arguments = new Arguments;
    for (int i = 0; i < argc; ++i) {
	    const std::string currentArg(argv[i]);
        if(currentArg == "-h" || currentArg == "--help"){
            arguments->flagHelp = true;
        } else if(currentArg == "-t"){
            arguments->flagTcp = true;
        } else if(currentArg == "-u"){
            arguments->flagUdp = true;
        } else if(currentArg == "-p" || currentArg == "--port") {
            if (++i >= argc) {
				Logger::Error("Arguments", "Missing port number argument");
                return nullptr;
            }
	        const std::string port(argv[i]);
            try {
                arguments->portNumber = stoi(port);
            } catch (...) {
				Logger::Error("Arguments", "Invalid port number argument");
                return nullptr;
            }
            if (arguments->portNumber < 0 || arguments->portNumber > 65535) {
				Logger::Error("Arguments", "Invalid port number argument");
                return nullptr;
            }
        } else if(currentArg == "-w" || currentArg == "--wait") {
            if (++i >= argc) {
				Logger::Error("Arguments", "Missing maximum RTT argument");
                return nullptr;
            }
	        const std::string wait(argv[i]);
            try {
                arguments->maxRtt = stoi(wait);
            } catch (...) {
				Logger::Error("Arguments", "Invalid maximum RTT argument");
                return nullptr;
            }
            if (arguments->maxRtt < 0) {
				Logger::Error("Arguments", "Invalid maximum RTT argument");
                return nullptr;
            }
        } else if(currentArg == "-n" || currentArg == "--network"){
            if(++i >= argc) {
				Logger::Error("Arguments", "Missing network describing argument");
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
    this->maxRtt = -1;
    this->portNumber = AllPorts;
}
