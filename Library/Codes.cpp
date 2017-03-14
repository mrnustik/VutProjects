//
// Created by mrnda on 3/14/17.
//

#include "Codes.h"


string codeToMessage(int code) {
    switch (code)
    {
        case CODE_OK:
            return "";
        case CODE_NOT_DIRECTORY:
            return "Not a directory\n";
        case CODE_DIR_NOT_FOUND:
            return "Directory not found\n";
        case CODE_NOT_EMPTY:
            return "Directory not empty\n";
        case CODE_EXISTS:
            return "Already exists\n";
        case CODE_NOT_FILE:
            return "Not a file\n";
        case CODE_FILE_NOT_FOUND:
            return "File not found\n";
        case CODE_USER_NOT_FOUND:
            return "User not found\n";
        case CODE_UNKNOWN:
        default:
            return "Code unknown\n";
    }
}
