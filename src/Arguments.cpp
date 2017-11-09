//
// Created by Michal Mrnustik on 26/09/2017.
//

#include "Arguments.h"
#include "Logger.h"
#include "NetworkHelper.h"

Arguments* Arguments::ParseArguments(int argc, char* argv[])
{
	auto* arguments = new Arguments;
	for (int i = 1; i < argc; ++i)
	{
		const std::string currentArg(argv[i]);
		if (currentArg == "-h" || currentArg == "--help")
		{
			arguments->flagHelp = true;
		}
		else if (currentArg == "-t")
		{
			arguments->flagTcp = true;
		}
		else if (currentArg == "-u")
		{
			arguments->flagUdp = true;
		}
		else if (currentArg == "-p" || currentArg == "--port")
		{
			if (++i >= argc)
			{
				Logger::Error("Arguments", "Missing port number argument");
				return nullptr;
			}
			const std::string port(argv[i]);
			try
			{
				arguments->portNumber = stoi(port);
			}
			catch (...)
			{
				Logger::Error("Arguments", "Invalid port number argument");
				return nullptr;
			}
			if (arguments->portNumber < 0 || arguments->portNumber > 65535)
			{
				Logger::Error("Arguments", "Invalid port number argument");
				return nullptr;
			}
		}
		else if (currentArg == "-w" || currentArg == "--wait")
		{
			if (++i >= argc)
			{
				Logger::Error("Arguments", "Missing maximum RTT argument");
				return nullptr;
			}
			const std::string wait(argv[i]);
			try
			{
				arguments->maxRtt = stoi(wait);
			}
			catch (...)
			{
				Logger::Error("Arguments", "Invalid maximum RTT argument");
				return nullptr;
			}
			if (arguments->maxRtt < 0)
			{
				Logger::Error("Arguments", "Invalid maximum RTT argument");
				return nullptr;
			}
		}
		else if (currentArg == "-n" || currentArg == "--network")
		{
			if (++i >= argc)
			{
				Logger::Error("Arguments", "Missing network describing argument");
				return nullptr;
			}
			const std::string network(argv[i]);
			try
			{
				arguments->network = IpNetwork::FromCidr(network);
			}
			catch (...)
			{
				Logger::Error("Arguments", "Unable to parse network IP. Please use CIDR formatting.");
				return nullptr;
			}
		}
		else if (currentArg == "-i" || currentArg == "--interface")
		{
			if (++i >= argc)
			{
				Logger::Error("Arguments", "Missing interface argument");
			}
			arguments->interfaceName = std::string(argv[i]);
			arguments->interfaceNumber = NetworkHelper::GetNetworkInterfaceNumber(arguments->interfaceName);
		}
		else
		{
			Logger::Error("Arguments", "Unexpected argument " + currentArg);
			return nullptr;
		}
	}
	Logger::Debug("Arguments", "Loaded");
	Logger::Debug("Arguments", "Help: " + std::to_string(arguments->flagHelp));
	Logger::Debug("Arguments", "TCP: " + std::to_string(arguments->flagTcp));
	Logger::Debug("Arguments", "UDP: " + std::to_string(arguments->flagUdp));
	Logger::Debug("Arguments", "Interface name: " + arguments->interfaceName);
	Logger::Debug("Arguments", "Interface number: " + std::to_string(arguments->interfaceNumber));
	Logger::Debug("Arguments", "IP network: " + arguments->network.ToString());
	Logger::Debug("Arguments", "Port number: " + std::to_string(arguments->portNumber));
	Logger::Debug("Arguments", "Maximum RTT: " + std::to_string(arguments->maxRtt));
	return arguments;
}

Arguments::Arguments() : network(IpNetwork::FromCidr("0.0.0.0/0"))
{
	this->flagHelp = false;
	this->flagTcp = false;
	this->flagUdp = false;
	this->networkString = std::string("");
	this->interfaceName = std::string("");
	this->interfaceNumber = 0;
	this->maxRtt = 10000;
	this->portNumber = AllPorts;
}
