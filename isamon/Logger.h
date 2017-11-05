#pragma once
#include <string>
#include <iostream>

enum Colour
{
	Red,
	Yellow,
	Green,
	Normal
};

class Logger
{
public:
	static void Debug(std::string category, std::string message);

	static void Error(std::string category, std::string message);
	static void Error(std::string category, std::string message, int errNumber);
	~Logger();
private:
	Logger();
	static void WriteOutput(Colour outputColour, std::string category, std::string message);
	static const bool IsDebug = true;
};

