
#include <fstream>
#include "Files.h"
#include "Codes.h"

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
    return 0;
}

int writeFile(string rootFolder, Url *url, string body)
{
    string userName = url->userName;
    string path = url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    ofstream OutFile;
    OutFile.open(fullPath, ios::out | ios::binary);
    if(OutFile.fail())
        return -1;
    OutFile.write(body.c_str(), body.length());
    OutFile.close();
    return 0;
}

int writeFile(string remotePath, string body)
{
    int lastIndex = remotePath.find_last_of(remotePath);
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
