
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

string getFileContentLength(string path)
{
    //TODO
    return "1000";
}
