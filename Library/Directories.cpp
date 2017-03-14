//
// Created by Michal Mrnustik on 11/03/2017.
//

#include <errno.h>

#include "Directories.h"

using namespace std;

#define DIR_OK              0
#define DIR_NOT_EXISTS      1
#define DIR_NOT_DIRECTORY   2

int directoryExists(string fullPath)
{
    struct stat state;
    if(stat(fullPath.c_str(), &state) == 0)
    {
        return DIR_NOT_EXISTS;
    }
    if(S_ISDIR(state.st_mode))
    {
        return DIR_OK;
    }
    return DIR_NOT_DIRECTORY;
}

tuple<int, vector<string>> getDirectoryContent(string rootFolder, Url* url)
{
    vector<string> directories;
    int code = 0;
    string userName = url->userName;
    string path = url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    int isDir = directoryExists(fullPath);
    if(isDir == DIR_OK)
    {
        code = CODE_OK;
    }
    else if(isDir == DIR_NOT_DIRECTORY)
    {
        code = CODE_NOT_DIRECTORY;
    }
    else
    {
        code = CODE_DIR_NOT_FOUND;
    }

    DIR* dir = opendir(fullPath.c_str());
    if(dir != NULL)
    {
        string name = "";
        struct dirent* entry;
        while((entry = readdir(dir)) != NULL)
        {
            name = entry->d_name;
            directories.push_back(name);
        }
        closedir(dir);
    }

    return make_tuple(code, directories);

};

int createDirectory(string rootFolder, Url* url)
{
    string userName = url->userName;
    string path = url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    int result = mkdir(fullPath.c_str(), 0700);
    if(result < 0){
       if(errno == EEXIST)
       {
           return CODE_EXISTS;
       }
       else
       {
           return CODE_UNKNOWN;
       }
    }
    return CODE_OK;
}

int deleteDirectory(string rootFolder, Url *url)
{
    string userName = url->userName;
    string path = url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    int result = rmdir(fullPath.c_str());
    if(result < 0){
        if(errno == ENOTEMPTY)
        {
            return CODE_NOT_EMPTY;
        }
        else if(errno == ENOTDIR)
        {
            return CODE_NOT_DIRECTORY;
        }
        else if(errno == ENOENT)
        {
            return CODE_DIR_NOT_FOUND;
        }
        else
        {
            return CODE_UNKNOWN;
        }
    }
    return CODE_OK;
}

string getCurrentDirectory()
{
    char temp[1024];
    return ( getcwd(temp, 1024) ? string( temp ) : string("") );
}
