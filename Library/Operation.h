//
// Created by Michal Mrnustik on 11/03/2017.
//

#ifndef OPERATION_H
#define OPERATION_H

#include <string>

using namespace std;

typedef enum {
    DIR_MAKE,
    DIR_REMOVE,
    DIR_LIST,
    FILE_UPLOAD,
    FILE_DELETE,
    FILE_GET,
    INVALID
} OperationType;


OperationType parseOperation(string str);




string operationToHTTPMethod(OperationType type);

string operationToAccept(OperationType type);

string operationToTypeQuery(OperationType types);

#endif //OPERATION_H
