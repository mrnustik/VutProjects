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
	static void Warning(std::string category, std::string message);
	static void Error(std::string category, std::string message);

	Logger();
	~Logger();
private:
	static void WriteOutput(Colour outputColour, std::string category, std::string message);
};

