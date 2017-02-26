//
// Created by Michal Mrnustik on 23/02/2017.
//

#ifndef OPERATION_H
#define OPERATION_H

#include <string.h>

typedef enum {
    DELETE,
    DOWNLOAD,
    UPLOAD,
    LIST,
    MAKE_DIR,
    REMOVE_DIR,
    INVALID
} OperationType;

OperationType parseOperation(char* opString);

const char* operationToHTTPMethod(OperationType operation);

#endif //OPERATION_H
