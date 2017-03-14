//
// Created by Michal Mrnustik on 11/03/2017.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>


using namespace std;

void disableLogging();
void logInfo(string category, string message);
void logWarning(string category, string message);
void logError(string category, string message);
#endif
