#include <iostream>
#include "Arguments.h"
using namespace std;

int main(int argc, char* argv[]) {
	const auto arguments = Arguments::parseArguments(argc, argv);
    if(arguments == nullptr){
        return 1;
    }
    return 0;
}