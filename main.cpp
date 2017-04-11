#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <openssl/crypto.h>
#include "Library/Socket.h"
#include "Library/MD5.h"
const std::string PortNumber = "55555";

using namespace std;


double parseExpression(string expression, bool *error);

string getMD5Hash(string userName);

int main(int argc, char* argv[]) {
    if(argc != 2)
    {
        std::cerr << "Invalid argument count" << endl;
        return EXIT_FAILURE;
    }
    const std::string address = argv[1];
    try {
        Socket *socket = new Socket();
        socket->connectSocket(address, PortNumber);
		string hash = getMD5Hash("xmrnus01");
        socket->send("HELLO "+hash+"\n");
        std::string data = "";
        while (true) {
            socket->recieve(data);
            if (data.find("BYE ") != string::npos) {
                cout << data.substr(4);
                break;
            } else if (data.find("SOLVE ") != string::npos) {
                data = data.substr(6);
                bool error = false;
                double result = 0;
                result = parseExpression(data, &error);
				if (error == false) {
					std::stringstream stream;
					stream << fixed << setprecision(2) << result;
					std::string result = stream.str();
					socket->send("RESULT " + result + "\n");
				}
                else
                    socket->send("RESULT ERROR\n");
            } else {
                cerr << "Invalid server message" << endl;
                exit(EXIT_FAILURE);
            }
        }
        socket->closeSocket();
    }
    catch (const char* ex)
    {
        cerr << ex << endl;
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

string getMD5Hash(string userName)
{
    return md5(userName);
}

double parseExpression(string expression, bool *error) {
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
    double result = 0;

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
