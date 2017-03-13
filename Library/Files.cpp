
#include <fstream>
#include "Files.h"
#include "Codes.h"

#define FILE_OK         0
#define FILE_NOT_EXIST  1
#define FILE_NOT_FILE   2

int fileExists(string fullPath)
{
    struct stat state;

    if(stat(fullPath.c_str(), &state) != 0)
    {
        return FILE_NOT_EXIST;
    }
    if(S_ISREG(state.st_mode))
    {
        return FILE_OK;
    }
    return FILE_NOT_FILE;
}

int deleteFile(string rootFolder, Url *url)
{
    string userName = url->userName;
    string path = url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    int result = unlink(fullPath.c_str());
    if(result < 0)
    {
        if(errno == ENOENT)
        {
            return CODE_FILE_NOT_FOUND;
        }
        else if(errno == EISDIR)
        {
            return CODE_NOT_FILE;
        }
    }
    return CODE_OK;
}

int writeFile(string rootFolder, Url *url, string body)
{
    string userName = url->userName;
    string path = url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    ofstream OutFile;
    OutFile.open(fullPath, ios::out | ios::binary);
    if(OutFile.fail())
        return CODE_EXISTS;
    OutFile.write(body.c_str(), body.length());
    OutFile.close();
    return 0;
}

pair<int, string> readFile(string rootFolder, Url *url)
{
    string userName = url->userName;
    string path = url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;

    int result = 0;
    string body = "";

    int exists = fileExists(fullPath);

    if(exists != FILE_OK)
    {
        result = CODE_FILE_NOT_FOUND;
    }
    else if(exists == FILE_NOT_FILE)
    {
        result = CODE_UNKNOWN;
    }
    else
    {
        result = CODE_OK;
        ifstream ifs(fullPath);
        body = string( (istreambuf_iterator<char>(ifs) ),
                        (istreambuf_iterator<char>()    ) );
    }

    return make_tuple(result, body);
}


int writeFile(string remotePath, string body)
{
    int lastIndex = remotePath.find_last_of("/");
    string fullPath = "./" + remotePath.substr(lastIndex+1);
    ofstream OutFile;
    OutFile.open(fullPath, ios::out | ios::binary);
    if(OutFile.fail())
        return -1;
    OutFile.write(body.c_str(), body.length());
    OutFile.close();
}

string getFileContentLength(string path)
{
    ifstream is;
    is.open (path.c_str(), ios::binary );
    is.seekg (0, ios::end);
    unsigned long length = is.tellg();
    return to_string(length);
}
