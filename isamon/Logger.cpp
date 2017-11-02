#include <cstring>
#include "Logger.h"



void Logger::Debug(const std::string category, const std::string message)
{
	WriteOutput(Green, category, message);
}

void Logger::Warning(const std::string category, const std::string message)
{
	WriteOutput(Yellow, category, message);
}

void Logger::Error(const std::string category, const std::string message)
{
	WriteOutput(Red, category, message);
}


void Logger::Error(std::string category, std::string message, int errNumber) {
	WriteOutput(Red, category, message);
	auto errorMessage = std::string(strerror(errNumber));
	WriteOutput(Red, "Message", errorMessage);
}


Logger::Logger()
{
}


Logger::~Logger()
{
}

void Logger::WriteOutput(Colour outputColour, std::string category, std::string message)
{
	if (!IsDebug) return;
	std::string colourFormat;
	switch (outputColour)
	{
	case Green:
		colourFormat = "\033[0;32m";
		break;
	case Red:
		colourFormat = "\033[0;31m";
		break;
	case Yellow:
		colourFormat = "\033[0;33m";
		break; 
	default:
		colourFormat = "\033[0;30m";
		break;
	}
	std::cerr << colourFormat + "[" + category + "]: " + message + "\033[0m" << std::endl;

}

