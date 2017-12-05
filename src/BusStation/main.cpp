
#include <iostream>
#include <fstream>
#include <simlib.h>

#include <vector>
#include <queue>

#include <string.h>


#define DEBUG true

#define START_OF_SIMULATION 60 * 0 // 8:00
#define END_OF_SIMULATION	60 * 24 //one day
#define SIMULATION_LENGTH END_OF_SIMULATION - START_OF_SIMULATION

#define NUMBER_OF_PLATFORMS 4

#define CONNECTION_BRNO					1

#define CONNECTION_PRAHA				2
#define AVERAGE_DELAY_PRAHA				9.36
#define PROBABILITY_DELAY_PRAHA			0.2

#define CONNECTION_KROMERIZ				3
#define AVERAGE_DELAY_KROMERIZ			4.8
#define PROBABILITY_DELAY_KROMERIZ		0.1

#define CONNECTION_BRATISLAVA			4
#define AVERAGE_DELAY_BRATISLAVA		13.33
#define PROBABILITY_DELAY_BRATISLAVA	0.2

#define CONNECTION_VIDEN				5
#define AVERAGE_DELAY_VIDEN				2.5
#define PROBABILITY_DELAY_VIDEN			0.05


#define PLATFORM_WAIT_TIME				10

Facility Platforms[NUMBER_OF_PLATFORMS];
Facility ArrivalRoad("Arrival road");
Queue PlatformsQueue;

Histogram DelayHistogram("Delay", 0, 1, 30);
Histogram LocalDelayHistogram("Local delay", 0, 1, 30);

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
	static void DebugLog(const std::string category, const std::string message)
	{
		if (DEBUG)
		{
			int time = (int)Time;
			int timeHours = time / 60;
			int timeMinutes = time % 60;
			std::cout << std::to_string(timeHours) << ":" << std::to_string(timeMinutes) << " [" << category << "]: " << message << std::endl;
		}
	}

	static void DelayLog(double time, double delay)
	{
		std::ofstream output;
		output.open(("stats.csv"), std::ofstream::out | std::ofstream::app);
		
		if(output.is_open())
		{
			int hours = time / 60;
			int minutes = ((int) time) % 60;
			output << hours << ":" << minutes << ";" << delay << std::endl;
			output.close();
			
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

		double roadAheadTime = Time;
		Seize(ArrivalRoad);
		double delayOnArrivalRoad = Time - roadAheadTime;
		delay += delayOnArrivalRoad;
		if(delayOnArrivalRoad > 0) 
		{
			LocalDelayHistogram(delayOnArrivalRoad);
		}
FindPlatform:
		int platformNumber = -1;
		for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {
			if (Platforms[i].Busy() == false) 
			{
				Release(ArrivalRoad);		
				Seize(Platforms[i]);
				platformNumber = i;
				break;
			}
		}

		if (platformNumber == -1) 
		{
			Wait(PLATFORM_WAIT_TIME);
			Release(ArrivalRoad);
			return;
		}

		Wait(PLATFORM_WAIT_TIME);
		Release(Platforms[platformNumber]);

		if (PlatformsQueue.Empty() == false) 
		{
			auto bus = PlatformsQueue.GetFirst();
			bus->Activate();
		}
		if(delay != 0){
			Logger::DelayLog(Time, delay);
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
			(new Bus(GetDelayProbability(_connection.directionNumber), 
					GetDelayForConnection(_connection.directionNumber))
				)->Activate();
			_connection.schedules.pop();
		}
		if (_connection.schedules.size() == 0) return;
		schedule = _connection.schedules.front();
		while(schedule.dispatchTime < Time)
		{
			if(_connection.schedules.size()	==  0) return;
			_connection.schedules.pop();
			schedule = _connection.schedules.front();
		}
		this->Activate(schedule.dispatchTime);
	}




	double GetDelayForConnection(int connectionNumber)
	{
		switch(connectionNumber){
			case CONNECTION_BRNO:
				return 0;
			case CONNECTION_PRAHA:
				return AVERAGE_DELAY_PRAHA;
			case CONNECTION_KROMERIZ:
				return AVERAGE_DELAY_KROMERIZ;
			case CONNECTION_BRATISLAVA:
				return AVERAGE_DELAY_BRATISLAVA;
			case CONNECTION_VIDEN:
				return AVERAGE_DELAY_VIDEN;
		}
		return 0;
	}

	double GetDelayProbability(int connectionNumber)
	{
		switch(connectionNumber)
		{
			case CONNECTION_BRNO:
				return 0;
			case CONNECTION_PRAHA:
				return PROBABILITY_DELAY_PRAHA;
			case CONNECTION_KROMERIZ:
				return PROBABILITY_DELAY_KROMERIZ;
			case CONNECTION_BRATISLAVA:
				return PROBABILITY_DELAY_BRATISLAVA;
			case CONNECTION_VIDEN:
				return PROBABILITY_DELAY_VIDEN;
		}
	}
};


int main(const int argc, char **argv) {
	
    int capacity=0;
    std::string source_file;
	double beginOfSimulation = START_OF_SIMULATION;
	double endOfSimulation = END_OF_SIMULATION;
    for (int i=1; i < argc; i++)        //parse arguments
    {
        if (strcmp(argv[i], "-h") == 0)
        {
            printf("Usage of this script:\n\n./SCRIPT -s path [-b 6:00] [-e 8:00] [-c 5] [-h]\n-h            >> prints this help\n-c capacity   >> sets capacity of stations\n-b time       >> begin of simulation\n-e time       >> end of simulation\n-s sourcefile >> source file for departure times (.csv)\n\n");
            exit (0);
        }
        else if (strcmp(argv[i], "-c") == 0)
        {
            char *ptr;
            capacity = strtol(argv[i+1], &ptr, 10);
            if (ptr[0] != '\0')
            {
                printf("Error while getting arguments\n");
                exit (1);
            }
            i++;
            continue;
        }
        else if (strcmp(argv[i], "-b") == 0)
        {
	        const std::string begin_sim = argv[i+1];
			auto time = Helper::split(begin_sim, ":");
			if(time.size() == 2)
			{
				try 
				{
					const int hours = std::stoi(time[0]);
					const int minutes = std::stoi(time[1]);
					beginOfSimulation = hours * 60 + minutes;
				}
				catch (...) 
				{
					std::cerr << "Invalid start time" << std::endl; 
					exit(1);
				}
			}
            i++;
            continue;
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
	        const std::string end_sim = argv[i+1];
			auto time = Helper::split(end_sim, ":");
			if (time.size() == 2)
			{
				try
				{
					const int hours = std::stoi(time[0]);
					const int minutes = std::stoi(time[1]);
					endOfSimulation = hours * 60 + minutes;
				}
				catch (...)
				{
					std::cerr << "Invalid start time" << std::endl;
					exit(1);
				}
			}
            i++;
            continue;
        }
        else if (strcmp(argv[i], "-s") == 0)
        {
            source_file = argv[i+1];
            i++;
            continue;
        }
        else
        {
            printf("error while getting arguments\n");
            exit (1);
        }

    }

    if (source_file.empty())
    {
        printf("No source file was specified, exiting...\n");
        exit (1);
    }

    if (capacity<1)
    {
        printf("Capacity cannot be less than 1, exiting...\n");
        exit (1);
    }
    
    Print("Brno Bus Station simulace\n");
	Init(beginOfSimulation, endOfSimulation);
	RandomSeed(time(nullptr));
	auto schedule = Schedule::LoadScheduleFromCsv(source_file);
	for(Connection& connection : schedule.GetAllConnections())
	{
		(new ScheduledGenerator(connection))->Activate();
	}
	Run();
	for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) 
	{
		Logger::DebugLog("Statistics", "Platform number " + std::to_string((i + 1)));
		Platforms[i].Output(); 
	}
	
 	ArrivalRoad.Output();
	DelayHistogram.Output();
	LocalDelayHistogram.Output();
}
