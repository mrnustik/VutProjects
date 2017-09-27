#include <iostream>
#include <string>
using namespace std;

typedef struct {

} tNetworkDefinition;

typedef struct {
    bool flagHelp;
    char* interfaceName;
    tNetworkDefinition* networkDefinition;
    bool flagTcp;
    bool flagUdp;
    int portNumber;
    int maxRtt;
} tArguments;

int parseArguments(int argc, char* argv[], tArguments* arguments)
{
    for (int i = 0; i < argc; ++i) {
        string parameter(argv[i]);
        if(parameter.compare("-h") || parameter.compare("--help")){
            arguments->flagHelp = true;
        }
    }
}


int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}