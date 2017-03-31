#include <iostream>
#include <string>
#include <cstdlib>
#include "Library/Socket.h"

const std::string PortNumber = "55555";

using namespace std;


long parseExpression(string expression, bool *error);


int main(int argc, char* argv[]) {
    if(argc != 2)
    {
        std::cerr << "Invalid argument count";
        return EXIT_FAILURE;
    }
    const std::string address = argv[1];
    try {
        Socket *socket = new Socket();
        socket->connectSocket(address, PortNumber);
        socket->send("HELLO md5Hash\n");
        std::string data = "";
        while (true) {
            socket->recieve(data);
            if (data.find("BYE ") != string::npos) {
                cout << data.substr(4);
                break;
            } else if (data.find("SOLVE ") != string::npos) {
                data = data.substr(6);
                bool error = false;
                int result = 0;
                result = parseExpression(data, &error);
                if(error == false)
                    socket->send("RESULT "+to_string(result)+"\n");
                else
                    socket->send("RESULT ERROR\n");
            } else {
                cerr << "Invalid server message";
                exit(EXIT_FAILURE);
            }
        }
        socket->closeSocket();
    }
    catch (const char* ex)
    {
        cerr << ex;
        exit(EXIT_FAILURE);
    }
    return 0;
}

typedef enum {
    ADD,
    SUB,
    DIV,
    MUL
} Operation;


long parseExpression(string expression, bool *error) {
    int i = 0;
    long leftOperand = 0;
    long rightOperand = 0;
    string numString = "";
    Operation o;
    unsigned long opPosition = -1;
    if((opPosition = expression.find('+')) != string::npos) o = ADD;
    else if((opPosition = expression.find('-')) != string::npos) o = SUB;
    else if((opPosition = expression.find('*')) != string::npos) o = MUL;
    else if((opPosition = expression.find('/')) != string::npos) o = DIV;
    else {
        *error = true;
        return 0;
    }
    char * tmp = NULL;
    try {
        numString = expression.substr(0, opPosition - 1);
        leftOperand = stol(numString);

        numString = expression.substr(opPosition + 1);
        rightOperand = stol(numString);
    }
    catch (...)
    {
        *error = true;
        return 0;
    }
    long result = 0;

    switch(o)
    {
        case ADD:
            result = leftOperand + rightOperand;
            break;
        case SUB:
            result = leftOperand - rightOperand;
            break;
        case MUL:
            result = leftOperand * rightOperand;
            break;
        case DIV:
            if(rightOperand == 0)
            {
                *error = true;
                return 0;
            }
            result = leftOperand / rightOperand;
    }

    return result;
}