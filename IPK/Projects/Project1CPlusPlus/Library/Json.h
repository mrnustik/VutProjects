//
// Created by mrnda on 3/14/17.
//

#ifndef JSON_H
#define JSON_H

#include <string>

using namespace std;

typedef struct {
    int code;
    string body;
} JsonMessage;

string jsonWithStatus(int code);
string jsonWithStatusAndBody(int code, string body);
JsonMessage* jsonFromString(string json);


#endif //JSON_H
