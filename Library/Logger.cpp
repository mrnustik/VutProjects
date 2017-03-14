//
// Created by Michal Mrnustik on 11/03/2017.
//


#include "Logger.h"

bool logging = true;

void disableLogging()
{
    logging = false;
}

void logInfo(string category, string message)
{
    if(logging)
        cout << " Info (" << category << ") : " << message << endl;
}

void logWarning(string category, string message)
{
    if(logging)
        cout << "Warning (" << category << ") : " << message << endl;
}

void logError(string category, string message)
{
    cerr << "Error (" << category << ") : " << message << endl;
}
