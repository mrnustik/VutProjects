
#include <fstream>
#include "Files.h"

int deleteFile(string rootFolder, HttpRequest *request)
{
    string userName = request->url->userName;
    string path = request->url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    int result = unlink(fullPath.c_str());
    if(result < 0)
    {
        if(errno == ENOENT)
        {
            return -20;
        }
        else if(errno == EISDIR)
        {
            return -30;
        }
    }
    return 0;
}

int writeFile(string rootFolder, HttpRequest *request, string body)
{
    string userName = request->url->userName;
    string path = request->url->path;
    string fullPath = rootFolder + "/" + userName + "/" + path;
    ofstream OutFile;
    OutFile.open(fullPath, ios::out | ios::binary);
    if(OutFile.fail())
        return -1;
    OutFile.write(body.c_str(), body.length());
    OutFile.close();
    return 0;
}

string getFileContentLength(string path)
{
    ifstream is;
    is.open (path.c_str(), ios::binary );
    is.seekg (0, ios::end);
    unsigned long length = is.tellg();
    return to_string(length);
}
