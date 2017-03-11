//
// Created by Michal Mrnustik on 11/03/2017.
//


#include "Logger.h"

void logInfo(string category, string message)
{
    cout << " Info (" << category << ") : " << message << endl;
}

void logWarning(string category, string message)
{
    cout << "Warning (" << category << ") : " << message << endl;
}

void logError(string category, string message)
{
    cout << "Error (" << category << ") : " << message << endl;
}
