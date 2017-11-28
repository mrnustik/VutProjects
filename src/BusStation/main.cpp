
#include <iostream>
#include <simlib.h>
#include <string.h>

#define DEBUG true

#define START_OF_SIMULATION 60 * 8 // 8:00
#define END_OF_SIMULATION	60 * 24 //one day
#define SIMULATION_LENGTH END_OF_SIMULATION - START_OF_SIMULATION

#define NUMBER_OF_PLATFORMS 6

#define AVERAGE_DELAY_PRAHA				10
#define PROBABILITY_DELAY_PRAHA			0.2
#define ARRIVAL_INTERVAL_PRAHA			15

#define AVERAGE_DELAY_KROMERIZ			5
#define PROBABILITY_DELAY_KROMERIZ		0.1
#define ARRIVAL_INTERVAL_KROMERIZ		25

#define AVERAGE_DELAY_BRATISLAVA		5
#define PROBABILITY_DELAY_BRATISLAVA	0.1
#define ARRIVAL_INTERVAL_BRATISLAVA		30

#define AVERAGE_DELAY_VIDEN				5
#define PROBABILITY_DELAY_VIDEN			0.1
#define ARRIVAL_INTERVAL_VIDEN			40

#define LOCAL_BUS_INTERVAL				5

#define PLATFORM_WAIT_TIME				15

Facility Platforms[NUMBER_OF_PLATFORMS];
Queue PlatformsQueue;

Histogram DelayHistogram("Delay", 0, SIMULATION_LENGTH, 1);
Histogram LocalDelayHistogram("Local delay", 0, SIMULATION_LENGTH, 1);

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

class Bus : public Process
{
	double delayValue = 0;
	double delayPercentage = 0;
	double delay = 0;
public:
	Bus(double delayPercentage, double delayValue) {
		this->delayPercentage = delayPercentage;
		this->delayValue = delayValue;
	}

	void Behavior()
	{
		if (Random() <= delayPercentage) 
		{
			double delayTime = Exponential(this->delayValue);
			Logger::DebugLog("Bus", "Delayed bus ariving " + std::to_string(delayTime));
			this->Wait(delayTime);
			delay += delayTime;
		}
		double startWaitingTime = -1;
FindPlatform:
		int platformNumber = -1;
		for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) {
			if (Platforms[i].Busy() == false) 
			{
				if (startWaitingTime > 0) 
				{
					auto localDelay = Time - startWaitingTime;
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
			startWaitingTime = Time;
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

class RemoteBusGenerator : public Event {
private:
	double delayValue = 0;
	double probabilityOfDelay = 0;
	std::string direction = "";
	double arrivalInterval = 0;
public:
	RemoteBusGenerator(std::string direction, double delayValue, double probabilityOfDelay, double arivalInterval) : Event()
	{
		this->direction = direction;
		this->delayValue = delayValue;
		this->probabilityOfDelay = probabilityOfDelay;
		this->arrivalInterval = arivalInterval;
	}

	void Behavior()
	{
		auto bus = new Bus(probabilityOfDelay, delayValue);
		Logger::DebugLog("Bus generator", "Bus arriving from " + direction);
		bus->Activate();
		this->Activate(Time + arrivalInterval);
	}
};


class LocalBusGenerator : public Event{
private:
	double arrivalInterval = 0;
public:
	LocalBusGenerator(double arrivalInterval)
	{
		this->arrivalInterval = arrivalInterval;
	}

	void Behavior() 
	{
		auto bus = new Bus(0, 0);
		Logger::DebugLog("Local bus generator", "Local bus arrived");
		bus->Activate();
		this->Activate(Time + arrivalInterval);
	}
};


int main(int argc, char **argv) {
	
    int capacity=0;
    std::string begin_sim, end_sim, source_file;
    
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
            begin_sim = argv[i+1];
            i++;
            continue;
        }
        else if (strcmp(argv[i], "-e") == 0)
        {
            end_sim = argv[i+1];
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
	Init(START_OF_SIMULATION, END_OF_SIMULATION);
	(new RemoteBusGenerator("Praha", AVERAGE_DELAY_PRAHA, PROBABILITY_DELAY_PRAHA, ARRIVAL_INTERVAL_PRAHA))->Activate();
	(new RemoteBusGenerator("Kroměříž", AVERAGE_DELAY_KROMERIZ, PROBABILITY_DELAY_KROMERIZ, ARRIVAL_INTERVAL_KROMERIZ))->Activate();
	(new RemoteBusGenerator("Bratislav", AVERAGE_DELAY_BRATISLAVA, PROBABILITY_DELAY_BRATISLAVA, ARRIVAL_INTERVAL_BRATISLAVA))->Activate();
	(new RemoteBusGenerator("Vídeň", AVERAGE_DELAY_VIDEN, PROBABILITY_DELAY_VIDEN, ARRIVAL_INTERVAL_VIDEN))->Activate();
	(new LocalBusGenerator(LOCAL_BUS_INTERVAL))->Activate();
	Run();
	for (int i = 0; i < NUMBER_OF_PLATFORMS; i++) 
	{
		Logger::DebugLog("Statistics", "Platform number " + std::to_string((i + 1)));
		Platforms[i].Output(); 
	}
	DelayHistogram.Output();
	LocalDelayHistogram.Output();
}
