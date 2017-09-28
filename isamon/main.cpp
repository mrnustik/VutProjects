#include <iostream>
#include "Arguments.h"
#include "Application.h"
using namespace std;

int main(int argc, char* argv[]) {
	const auto arguments = Arguments::ParseArguments(argc, argv);
    if(arguments == nullptr){
        return 1;
    }
	Application application(arguments);
    return 0;
}