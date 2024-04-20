

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>         
#include <sys/stat.h>      
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/shm.h>

#include "helpers.h"
#include "errors.h"
#include "constants.h"

#define EXIT_OK 0
#define EXIT_ARGUMENTS_FAIL 1
#define EXIT_SYS_CALL_FAIL 2

#define DEBUG 1

typedef struct {
	int actionCounter;
	int passengersLeft;
	int passengersCount;
	int leftCarCapacity;	
} TSharedMemory;

typedef struct {
	int passengers;
	int carCapacity;
	int passeGenSleep;
	int runSleep;
	FILE* file;
} TParams;

void deinitSempahores(){
	sem_unlink(SEM_BOARDING_MUTEX);
	sem_unlink(SEM_UNLOADING_MUTEX);
	sem_unlink(SEM_OUTPUT_MUTEX);
	sem_unlink(SEM_BOARD_QUEUE);
	sem_unlink(SEM_UNBOARD_QUEUE);
	sem_unlink(SEM_ALL_ABOARD);
	sem_unlink(SEM_ALL_UNLOAD);
	sem_unlink(SEM_FINISHED);
}

int initSemaphores(){
	sem_t *sem;
	
	if((sem = sem_open(SEM_ALL_ABOARD, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_close(sem);
	if((sem = sem_open(SEM_ALL_UNLOAD, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_close(sem);
	
	if((sem = sem_open(SEM_BOARDING_MUTEX, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_post(sem);
	sem_close(sem);
	
	if((sem = sem_open(SEM_BOARD_QUEUE, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_close(sem);

	if((sem = sem_open(SEM_OUTPUT_MUTEX, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_close(sem);
	
	if((sem = sem_open(SEM_UNBOARD_QUEUE, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_close(sem);
	
	if((sem = sem_open(SEM_UNLOADING_MUTEX, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_post(sem);
	sem_close(sem);

		
	if((sem = sem_open(SEM_FINISHED, O_CREAT, 0666, 0)) == SEM_FAILED){
		deinitSempahores();
		return -1;
	}
	sem_close(sem);

    return 0;
}



void car(TParams* params, TSharedMemory* memory){
	
	sem_t* output;
	sem_t* boardQueue;
	sem_t* allAboard;
	sem_t* unloadQueue;
	sem_t* allAshore;
	sem_t* finished;
	
	output = sem_open(SEM_OUTPUT_MUTEX, O_RDWR);
	boardQueue = sem_open(SEM_BOARD_QUEUE, O_RDWR);
	allAboard = sem_open(SEM_ALL_ABOARD, O_RDWR);
	unloadQueue = sem_open(SEM_UNBOARD_QUEUE, O_RDWR);
	allAshore = sem_open(SEM_ALL_UNLOAD, O_RDWR);
	finished = sem_open(SEM_FINISHED, O_RDWR);
	srand(getpid());
	
	
	fprintf(params->file, "%d\t: C 1	: started\n", memory->actionCounter++);
	fflush(params->file);
	
	while(memory->passengersLeft > 0){
		fprintf(params->file, "%d\t: C 1	: load\n", memory->actionCounter++);
		fflush(params->file);
		sem_post(output);
		for(int i = 0; i < params->carCapacity; i++){
			sem_post(boardQueue);
		}
		sem_wait(allAboard);
		sem_wait(output);
		fprintf(params->file,"%d\t: C 1	: run\n", memory->actionCounter++);
		fflush(params->file);
		sem_post(output);
		if(params->runSleep != 0){
			int randomTime = (rand() % (params->runSleep * 1000 + 1)); 
			usleep(randomTime);
		} 
		sem_wait(output);
		fprintf(params->file,"%d\t: C 1	: unload\n", memory->actionCounter++);
		fflush(params->file);
		sem_post(output);
		for(int i = 0; i < params->carCapacity; i++){
			sem_post(unloadQueue);
		} 
		sem_wait(allAshore);
	}
	sem_wait(output);
	fprintf(params->file, "%d	: C 1	: finished\n", memory->actionCounter++);
	fflush(params->file);
	sem_post(output);
	sem_post(finished);
	sem_close(output);
	sem_close(boardQueue);
	sem_close(allAboard);
	sem_close(unloadQueue);
	sem_close(allAshore);
	sem_close(finished);
}

void passenger(TParams* params, TSharedMemory* memory){
	sem_t* output;
	sem_t* boardQueue;
	sem_t* allAboard;
	sem_t* unloadQueue;
	sem_t* allAshore;
	sem_t* boardingMutex;
	sem_t* unboardingMutex;
	sem_t* finished;
	int id = 0;
	output = sem_open(SEM_OUTPUT_MUTEX, O_RDWR);
	boardQueue = sem_open(SEM_BOARD_QUEUE, O_RDWR);
	allAboard = sem_open(SEM_ALL_ABOARD, O_RDWR);
	unloadQueue = sem_open(SEM_UNBOARD_QUEUE, O_RDWR);
	allAshore = sem_open(SEM_ALL_UNLOAD, O_RDWR);
	boardingMutex = sem_open(SEM_BOARDING_MUTEX, O_RDWR);
	unboardingMutex = sem_open(SEM_UNLOADING_MUTEX, O_RDWR);
	finished = sem_open(SEM_FINISHED, O_RDWR);
	sem_wait(output);
	id = memory->passengersCount + 1;
	memory->passengersCount += 1;
	fprintf(params->file, "%d	: P %d	: started\n", memory->actionCounter++, id);
	fflush(params->file);
	sem_post(output);
	sem_wait(boardQueue);
	sem_wait(boardingMutex);
	sem_wait(output);
	fprintf(params->file, "%d	: P %d	: board\n", memory->actionCounter++, id);
	fflush(params->file);
	if(memory->leftCarCapacity - 1 != 0){
		int boardOrder = params->carCapacity - memory->leftCarCapacity + 1;
		fprintf(params->file, "%d	: P %d	: board order %d\n", memory->actionCounter++, id, boardOrder);
	} else {
		fprintf(params->file, "%d	: P %d	: board last\n", memory->actionCounter++, id);
		sem_post(allAboard);
	}
	fflush(params->file);
	memory->leftCarCapacity--;
	sem_post(output);
	sem_post(boardingMutex);
	sem_wait(unloadQueue);
	sem_wait(unboardingMutex);
	sem_wait(output);
	fprintf(params->file, "%d	: P %d	: unboard\n", memory->actionCounter++, id);
	fflush(params->file);
	if(memory->leftCarCapacity != params->carCapacity - 1){
		int unboardOrder = memory->leftCarCapacity + 1;
		fprintf(params->file, "%d	: P %d	: unboard order %d\n", memory->actionCounter++, id, unboardOrder);
		memory->passengersLeft--;
	} else {
		fprintf(params->file, "%d	: P %d	: unboard last\n", memory->actionCounter++, id);
		memory->passengersLeft--;
		sem_post(allAshore);
	}
	fflush(params->file);
	memory->leftCarCapacity++;
	sem_post(output);
	sem_post(unboardingMutex);
	
	sem_wait(finished);
	sem_wait(output);
	fprintf(params->file, "%d	: P %d	: finished\n", memory->actionCounter++, id);
	fflush(params->file);
	sem_post(output);
	sem_post(finished);
	sem_close(finished);
	sem_close(output);
	sem_close(unloadQueue);
	sem_close(boardQueue);
	sem_close(allAboard);
	sem_close(allAshore);
	sem_close(boardingMutex);
	sem_close(unboardingMutex);
	
}


void god(TParams* params, TSharedMemory* memory){
	srand(getpid());
	pid_t* pids = malloc(sizeof(pid_t)*params->passengers);	
	for(int i = 0; i < params->passengers; i++){
		if(params->passeGenSleep != 0){
			int sleepTime = (rand() % (params->passeGenSleep * 1000 + 1));
			usleep(sleepTime);
		}
		int pid = fork();
		if(pid < 0 ){
			for(int j = i - 1; j <= 0; j--){
				kill(pids[j], SIGKILL);
			}
			free(pids);
			exit(20);
		}
		if(pid == 0){ // passenger
			passenger(params, memory);
			exit(0);
		} else {
			pids[i] = pid;
		}
	}
	free(pids);
}

void freeMemory(TSharedMemory* memory, int shmMemId){

	shmdt(memory);
	shmctl(shmMemId, IPC_RMID, NULL);
}

int main(int argc, char* argv[])
{
	int passengers = 0;
	int carCapacity = 0;
	int passeGenSleep = 0;
	int runSleep = 0;
	int pid=0;
	pid_t carPid;
	pid_t godPid;
	
	int shmMemId = 0;
	key_t shmMemKey;
	TSharedMemory* memory = NULL;
	
	
	if(argc != 5) {
		printError("Add all arguments please");
		return 1;
	}
	char* strPassenger =  argv[1];
	char* strCapacity = argv[2];
	char* strPasseGenSleep = argv[3];
	char* strRunSleep = argv[4];

	if((passengers = stringToUInt(strPassenger)) <= 0)
	{
		printError("Passenger count must be number greater than zero. ");
		return EXIT_ARGUMENTS_FAIL;
	}

	if(((carCapacity = stringToUInt(strCapacity)) <= 0) || !(passengers % carCapacity == 0)){
		printError("Car capacity must be greater than zero. And passenger count should multiplication of carCapacity.");
		return EXIT_ARGUMENTS_FAIL;
	}

	if(((passeGenSleep = stringToUInt(strPasseGenSleep)) < 0) || passeGenSleep > 5001){
		printError("Passenger generation sleep time should be 0 or greater and lesser than 5001");
		return EXIT_ARGUMENTS_FAIL;
	}

	if(((runSleep = stringToUInt(strRunSleep)) < 0) || runSleep> 5001){
		printError("Car run sleep time should be 0 or greater and lesser than 5001");
		return EXIT_ARGUMENTS_FAIL;
	}
	
	TParams params = {
		.passengers = passengers,
		.carCapacity = carCapacity,
		.passeGenSleep = passeGenSleep,
		.runSleep = runSleep,
	};
	FILE* file;
	if((file = fopen("proj2.out","w")) == NULL){
		printError("An error occured while creating output file");
		return EXIT_SYS_CALL_FAIL;
	}
	params.file = file;
	
	if((shmMemKey = ftok(SHM_TOKEN, 1)) == (key_t) -1 ){
		printError("An error occured while getting token for shared memory");
		fclose(params.file);
		return EXIT_SYS_CALL_FAIL;
	}  
	
	if((shmMemId = shmget(shmMemKey, sizeof(TSharedMemory), IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR)) == -1){
		printError("An error occured while creating shared memory");
		fclose(params.file);
		return EXIT_SYS_CALL_FAIL;
	}
	
	if((memory = (TSharedMemory*) shmat(shmMemId, NULL, 0)) == (void *) -1){
		printError("An error occured while accessing shared memory");
		freeMemory(memory, shmMemId);
		fclose(params.file);
		return EXIT_SYS_CALL_FAIL;
	}
	
	memory->actionCounter = 1;
	memory->leftCarCapacity = params.carCapacity;
	memory->passengersLeft = params.passengers;
	memory->passengersCount = 0;
	
	initSemaphores();
	
	if((pid = fork()) < 0) {
		printError("An error occured while making car process");
		freeMemory(memory, shmMemId);
		deinitSempahores();
		fclose(params.file);
		return EXIT_SYS_CALL_FAIL;
	}
	
	if(pid == 0) { //Car process
		car(&params, memory);
		exit(0);
	} else {
		carPid = pid;
		if((pid = fork()) < 0){
			kill(carPid, SIGKILL);
			freeMemory(memory, shmMemId);
			deinitSempahores();
			fclose(params.file);
			return EXIT_SYS_CALL_FAIL;
		}
		
		if(pid == 0){ //God the creator process
			god(&params, memory);
			exit(0);
		} else {
			godPid = pid;			
		}
	}
	(void) godPid;
	
	int status;
	while (1) {
		pid = waitpid(-1, &status, 0);

		if(status == 5120){
			printError("Error during process creation.");
			kill(carPid, SIGKILL);
			deinitSempahores();
			freeMemory(memory, shmMemId);
			fclose(params.file);
			return EXIT_SYS_CALL_FAIL;
		}
		if (errno == ECHILD) {
			break;
		}
		

	}
	
	fclose(params.file);
	deinitSempahores();
	freeMemory(memory, shmMemId);
	
	return 0;
}
