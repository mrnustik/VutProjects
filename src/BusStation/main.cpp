
#include <iostream>
#include <fstream>
#include <simlib.h>
#include <vector>
#include <queue>

#define DEBUG true

#define START_OF_SIMULATION 60 * 0 // 8:00
#define END_OF_SIMULATION	60 * 24 //one day
#define SIMULATION_LENGTH END_OF_SIMULATION - START_OF_SIMULATION

#define NUMBER_OF_PLATFORMS 4

#define AVERAGE_DELAY_PRAHA				10
#define PROBABILITY_DELAY_PRAHA			0.2

#define AVERAGE_DELAY_KROMERIZ			5
#define PROBABILITY_DELAY_KROMERIZ		0.1

#define AVERAGE_DELAY_BRATISLAVA		5
#define PROBABILITY_DELAY_BRATISLAVA	0.1

#define AVERAGE_DELAY_VIDEN				5
#define PROBABILITY_DELAY_VIDEN			0.1

#define PLATFORM_WAIT_TIME				10

Facility Platforms[NUMBER_OF_PLATFORMS];
Queue PlatformsQueue;

Histogram DelayHistogram("Delay", 0, SIMULATION_LENGTH, 1);
Histogram LocalDelayHistogram("Local delay", 0, SIMULATION_LENGTH, 1);


class Helper {
public:
	static std::vector<std::string> split(const std::string& str, const std::string& delim)
	{
		std::vector<std::string> tokens;
		size_t prev = 0, pos = 0;
		do
		{
			pos = str.find(delim, prev);
			if (pos == std::string::npos) pos = str.length();
			std::string token = str.substr(prev, pos - prev);
			if (!token.empty()) tokens.push_back(token);
			prev = pos + delim.length();
		} while (pos < str.length() && prev < str.length());
		return tokens;
	}
};

class Logger 
{
public:
	static void DebugLog(std::string category, std::string message)
	{
		if (DEBUG)
		{
			int time = (int)Time;
			int timeHours = time / 60;
			int timeMinutes = time % 60;
			std::cout << std::to_string(timeHours) << ":" << std::to_string(timeMinutes) << " [" << category << "]: " << message << std::endl;
		}
	}
};

typedef struct ScheduleItem {
	double dispatchTime;
} tScheduleItem;

typedef struct Connection{
	int directionNumber; 
	std::string name;
	std::queue<tScheduleItem> schedules;
} tConnection;

class Schedule
{
	std::vector<Connection> _connections;
public:
	Schedule() : _connections() {}
	~Schedule() {}

	void AddToSchedule(const int direction, const std::string connectionName, const std::queue<ScheduleItem> schedules)
	{
		tConnection connection;
		connection.name = std::string(connectionName);
		connection.directionNumber = direction;
		connection.schedules = schedules;
		_connections.push_back(connection);
	}

	std::vector<Connection>& GetAllConnections()
	{
		return _connections;
	}

	static Schedule LoadScheduleFromCsv(const std::string path)
	{
		std::ifstream fileStream(path);
		if (!fileStream.is_open()) throw "Invalid file path";

		Schedule schedule{};
		std::string line;
		while(getline(fileStream, line))
		{
			auto values = Helper::split(line, ";");
			const int direction = std::stoi(values[0]);
			const std::string connectionName = values[1];
			std::queue<ScheduleItem> scheduleItems;
			for(unsigned int i = 2; i < values.size(); i++)
			{
				auto time = Helper::split(values[i], ":");
				if(time.size() == 2)
				{
					const int hours = std::stoi(time[0]);
					const int minutes = std::stoi(time[1]);
					tScheduleItem item;
					item.dispatchTime = hours * 60 + minutes;
					scheduleItems.push(item);
				}
			}
			schedule.AddToSchedule(direction, connectionName, scheduleItems);
		}
		fileStream.close();
		return schedule;
	}
};


class Bus : public Process
{
	double delayValue = 0;
	double delayPercentage = 0;
	double delay = 0;
public:
	Bus(double delayPercentage, double delayValue) : Process() {
		this->delayPercentage = delayPercentage;
		this->delayValue = delayValue;
	}

	void Behavior() override
	{
		if (Random() <= delayPercentage) 
		{
			const double delayTime = Exponential(this->delayValue);
			Logger::DebugLog("Bus", "Delayed bus ariving " + std::to_string(delayTime));
			Wait(delayTime);
			delay += delayTime;
		}

		double waitingStart = -1;
FindPlatform:
		int platformNumber = -1;
		for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {
			if (Platforms[i].Busy() == false) 
			{
				if (waitingStart > 0) 
				{
					auto localDelay = Time - waitingStart;
					LocalDelayHistogram(localDelay);
					delay += localDelay;
				}
				Seize(Platforms[i]);
				platformNumber = i;
				break;
			}
		}

		if (platformNumber == -1) 
		{
			Into(PlatformsQueue);
			waitingStart = Time;
			Passivate();
			goto FindPlatform;
		}

		Wait(PLATFORM_WAIT_TIME);

		Release(Platforms[platformNumber]);
		if (PlatformsQueue.Empty() == false) 
		{
			auto bus = PlatformsQueue.GetFirst();
			bus->Activate();
		}

		DelayHistogram(delay);
	}

};

class ScheduledGenerator : public Event
{
private:
	tConnection& _connection;
public:
	ScheduledGenerator(tConnection& connection) : _connection(connection)
	{
	}

	void Behavior() override
	{
		if (_connection.schedules.size() == 0) return;
		auto schedule = _connection.schedules.front();
		if(schedule.dispatchTime == Time)
		{
			Logger::DebugLog("Scheduled generator", "Bus coming from connection " + _connection.name);
			(new Bus(0, 0))->Activate();
			_connection.schedules.pop();
		}
		if (_connection.schedules.size() == 0) return;
		schedule = _connection.schedules.front();
		if(schedule.dispatchTime < Time)
		{
			std::cout << "Test";
		}
		this->Activate(schedule.dispatchTime);
	}
};


int main() {
	Print("Brno Bus Station simulace\n");
	Init(START_OF_SIMULATION, END_OF_SIMULATION);
	Schedule schedule = Schedule::LoadScheduleFromCsv("data.csv");
	for(tConnection &connection :  schedule.GetAllConnections())
	{
		(new ScheduledGenerator(connection))->Activate();
	}
	Run();
	for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) 
	{
		Logger::DebugLog("Statistics", "Platform number " + std::to_string((i + 1)));
		Platforms[i].Output(); 
	}
	DelayHistogram.Output();
	LocalDelayHistogram.Output();
}