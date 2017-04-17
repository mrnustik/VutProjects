#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <cmath>
#include "Library/Socket.h"

const std::string Login = "xmrnus01";
const std::string PortNumber = "55555";

using namespace std;
//--------- function declaration ---------------------
double truncateNumber(double In, unsigned int Digits);

double parseExpression(string expression, bool *error);

string getMD5Hash(string userName);


//--------- function implementation ---------------------
int main(int argc, char* argv[]) {
    if(argc != 2)
    {
        std::cerr << "Invalid argument count" << endl;
        return EXIT_FAILURE;
    }
    const std::string address = argv[1];
    try {

	//creates socket wrapper
        Socket *socket = new Socket();

	//connects to socket defined in address
        socket->connectSocket(address, PortNumber);

	//count the hash and send HELLO hash
	string hash = getMD5Hash(Login);
        socket->send("HELLO "+hash+"\n");

	//read and parse the data
        std::string data = "";
        while (true) {
  	    //wrapper returns received data or empty string if connection closes
            socket->recieve(data);
	     
	    //check for BYE or SOLVE message
            if (data.find("BYE ") != string::npos) {
		//print the "secret"
                cout << data.substr(4);
                break;
            } else if (data.find("SOLVE ") != string::npos) {
		//get the equation 
                data = data.substr(6);
                bool error = false;
                double result = 0;
		//try parse the expression
                result = parseExpression(data, &error);				
		if (error == false) 
		{
			//get the result in defined precision
			std::stringstream stream;
			stream << fixed << setprecision(2) << result;
			std::string result = stream.str();

			//send the result
			socket->send("RESULT " + result + "\n");
		}
                else 
		{	
		    //or send the error
                    socket->send("RESULT ERROR\n");
		}
            } else {
                cerr << "Invalid server message" << endl;
                exit(EXIT_FAILURE);
            }
        }
        socket->closeSocket();
    }
    catch (const char* ex) //catch errors from the wrapper class
    {
        cerr << ex << endl;
        exit(EXIT_FAILURE);
    }
    return 0;
}
//enum used to represent math operations when counting
typedef enum {
    ADD,
    SUB,
    DIV,
    MUL
} Operation;

//C++ wrapper around MD5 function of openssl
string getMD5Hash(string userName)
{
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)userName.c_str(), userName.length(), digest);
    char mdString[33];
    for(int i = 0; i < 16; i++)
	sprintf(&mdString[i*2], "%02x", (unsigned int) digest[i]);

    string result = "";
    result.append(mdString);	
    return result;
}


//parse expression and return result or *error set to true
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
            result = ((double) leftOperand) / ((double)rightOperand);
    }

    return truncateNumber(result, 2);
}


//function used to truncate number on x Digits
double truncateNumber(double In, unsigned int Digits)
{
    double f=pow(10, Digits);
    return ((int)(In*f))/f;
}
