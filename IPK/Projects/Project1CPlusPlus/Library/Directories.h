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
#include <vector>
#include <dirent.h>
#include "Codes.h"

int createDirectory(string rootFolder, Url* request);

tuple<int, vector<string>> getDirectoryContent(string rootFolder, Url* url);

int deleteDirectory(string rootFolder, Url* request);

string getCurrentDirectory();



#endif //DIRECTORIES_H
