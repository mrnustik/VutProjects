#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <cmath>
#include "Library/Command.h"
#include "Library/Socket.h"

const std::string Login = "xmrnus01";
const std::string PortNumber = "55555";

using namespace std;
//--------- function declaration ---------------------

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
                Command* command = Command::parse(data);
                if(command != nullptr)
                {
                    result = command->execute(&error);
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
                }
            } else {
                //ignore
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



