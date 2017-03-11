//
// Created by Michal Mrnustik on 11/03/2017.
//

#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include "Http.h"
#include <string>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


int createDirectory(string rootFolder, HttpRequest* request);

int deleteDirectory(string rootFolder, HttpRequest* request);

string getCurrentDirectory();



#endif //DIRECTORIES_H
