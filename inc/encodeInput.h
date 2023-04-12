/*
*	FILE			: encodeInput.h
*	PROJECT			: EncodeInput Util
*	PROGRAMMERS		: Sebastian Posada
					  Jonathon Gregoric
*	FIRST VERSION	: 2023-01-30
*	DESCRIPTION		: This file contains all constants and prototypes
*					  for the encodeInput utility
*/

// Header files used
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// constants
#define ERROR -1
#define STR_LEN 41
#define SREC 1
#define HELP 3
#define ADDRESS_SIZE 2
#define BYTE_SIZE 1
#define LINE_SIZE 16

// prototypes
int parseCmdLine(int argc, char* argv[]);
int fileStream(char* inputName, char* outputName, int srecSwitch);
int srecMode(FILE* readStream, FILE* writeStream);
int checkSUM(int count, int address, int sumDataField);
int asmMode(FILE* readStream, FILE* writeStream);
int showHelp(void);
