//
// Created by Michal Mrnustik on 11/03/2017.
//

#ifndef FILES_H
#define FILES_H

#include <string>
#include <unistd.h>
#include <errno.h>
#include "Http.h"

using namespace std;

int deleteFile(string rootFolder, HttpRequest* request);
int writeFile(string rootFolder, HttpRequest* request, string body);

string getFileContentLength(string path);

#endif //FILES_H
