//
// Created by mrnda on 3/12/17.
//

#ifndef CODES_H
#define CODES_H

#include <string>
using namespace std;

#define CODE_OK             0
#define CODE_NOT_DIRECTORY  501
#define CODE_DIR_NOT_FOUND  502
#define CODE_NOT_EMPTY      503
#define CODE_EXISTS         504
#define CODE_NOT_FILE       505
#define CODE_FILE_NOT_FOUND 506
#define CODE_USER_NOT_FOUND 507
#define CODE_UNKNOWN        600

string codeToMessage(int code);

#endif
