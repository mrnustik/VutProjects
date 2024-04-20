

#include "helpers.h"

int stringToUInt(char* str){
	long returnValue = strtol(str, (char**)NULL , 10);
	if(errno == ERANGE){
		return -1;
	} else if(returnValue > INT_MAX){
		return -1;
	} else {
		return (int) returnValue;
	}
}	

void printDebug(char* variableName, int value){
	printf("%s : %d\n", variableName, value);
}
