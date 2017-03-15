//
// Created by mrnda on 3/14/17.
//


#include "Json.h"
#include "Codes.h"

string jsonWithStatus(int code) {
    string json = "";

    json += "{\n";
    json += "\t\"status\":\"" + to_string(code) + "\"\n";
    json += "}\n";

    return json;
}

string jsonWithStatusAndBody(int code, string body) {
    string json = "";

    json += "{\n";
    json += "\t\"status\":\"" + to_string(code) + "\"\n,";
    json += "\t\"body\":\"" + body + "\"\n";
    json += "}\n";


    return json;
}

JsonMessage* jsonFromString(string json) {
    JsonMessage* message = new JsonMessage;

    int statusIndex = json.find("status\":");
    int endIndex = json.find("\"\n", statusIndex);
    if(statusIndex == string::npos)
        message->code = CODE_UNKNOWN;
    else
    {
        string codeString = json.substr(statusIndex + 9, endIndex - statusIndex - 9);
        try
        {
            message->code = stoi(codeString);
        }
        catch (...)
        {
            message->code = CODE_UNKNOWN;
        }
    }

    int bodyIndex = json.find("\"body\":");
    int bodyEndIndex = json.find("\"\n", bodyIndex);
    if(bodyIndex == string::npos)
        message->body = "";
    else
    {
        string codeString = json.substr(bodyIndex + 8, bodyEndIndex - bodyIndex - 8);
        message->body = codeString;
    }

    return message;
}
