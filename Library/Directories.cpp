//
// Created by Michal Mrnustik on 11/03/2017.
//

#include <errno.h>
#include "Directories.h"
#include "Http.h"

using namespace std;

int createDirectory(string rootFolder, HttpRequest* request)
{
    string userName = request->url->userName;
    string path = request->url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    int result = mkdir(fullPath.c_str(), 0700);
    if(result < 0){
       if(errno == EEXIST)
       {
           return -2;
       }
       if(errno == ENOENT)
       {
           return -3;
       }

    }
    return 0;
}

int deleteDirectory(string rootFolder, HttpRequest *request)
{
    string userName = request->url->userName;
    string path = request->url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    int result = rmdir(fullPath.c_str());
    if(result < 0){
        if(errno == EEXIST || errno == ENOTEMPTY)
        {
            return -2;
        }
        else if(errno == ENOTDIR)
        {
            return -3;
        }
        else
        {
            return -1;
        }
    }
    return 0;
}

string getCurrentDirectory()
{
    char temp[1024];
    return ( getcwd(temp, 1024) ? string( temp ) : string("") );
}
