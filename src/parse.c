/*
*	FILE			: parse.c
*	PROJECT		: EncodeInput Util
*	PROGRAMMERS		: Sebastian Posada
				  Jonathon Gregoric
*	FIRST VERSION		: 2023-01-30
*	DESCRIPTION		: This file contains functions to handle command line arguments
*				  and decide how to encode the binary file passed by the user.
*/

#include "../inc/encodeInput.h"
#pragma warning(disable : 4996)


/*
	FUNCTION	:	parseCmdLine
	DESCRIPTION	:	Parse command line arguments and determine which
					switches are being used. This will determine what
					the output format should be
	PARAMETERS	:	int argc		: count of how many arguments there are
					char* argv[]	: command line arguments
	RETURNS		:	0 if no errors are present, -1 otherwise

*/
int parseCmdLine(int argc, char* argv[])
{
	// the encodeInput utility allows the following swithes:
	//	-iINPUTFILENAME -oOUTPUTFILENAME -srec -h
	if (argc > 4)
	{
		return ERROR;
	}

	// character arrays to store filename input and output
	char inputFileName[STR_LEN] = "NULL";
	char outputFileName[STR_LEN] = "NULL";

	// buffer for copying strings
	char buffer[STR_LEN] = "";

	// if no switches are passed, default encoding option will occur
	if (argc == 1)
	{
		fileStream(inputFileName, outputFileName, 0);

		return 0;
	}


	// variables to store flags set to 0, if present they will be set to 1
	int inputSwitch = 0;
	int outPutSwitch = 0;
	int srecSwitch = 0;

	// iterate through the arguments to see what switches are to be set
	for (int i = 1; i < argc; ++i)
	{
		// copy the string into the buffer we created
		//strcpy_s(buffer, strlen(argv[i]) +1, argv[i]);
		strcpy(buffer, argv[i]);

		// check if the help switch has been set
		if (strcmp(buffer,"-h") ==0)
		{
			return showHelp();
		}

		// if the -i switch is present, copy the input file name entered
		else if (strncmp(buffer,"-i",2) == 0)
		{
			//strcpy_s(inputFileName, strlen(argv[i] + 1), argv[i]+2);
			strcpy(inputFileName, argv[i]+2);
			inputSwitch = 1;
		}

		// if the -o switch is present, copy the output file name entered
		else if (strncmp(buffer,"-o",2)==0)
		{
			//strcpy_s(outputFileName, strlen(argv[i] + 1), argv[i] + 2);
			strcpy(outputFileName,argv[i]+2);
			outPutSwitch = 1;
		}

		// if the -srec switch is present, encode file into the S-Record file
		else if (strcmp(buffer,"-srec")==0)
		{
			srecSwitch = 1;
		}
		else
		{
			return ERROR;
		}
	}

	if (inputSwitch <= 1 && outPutSwitch <= 1)
	{
		fileStream(inputFileName, outputFileName, srecSwitch);
		return 0;
	}

	else
	{
		return ERROR;
	}


}


/*
	FUNCTION	:	fileStream
	DESCRIPTION	:	This function will create a file stream for reading and writing to and
					from files. If no file name is present, the default stdin or stdout 
					are to be used.
	PARAMETERS	:	char* inputFileName		: input file name
					char* outputFileName	: output file name
					int srecSwitch			: 0 if not present, 1 if present
	RETURNS		:	0 if no errors are present, -1 otherwise

*/
int fileStream(char* inputFileName, char* outputFileName, int srecSwitch)
{
	// find out the input name and out name of the files for the writing stream
	char noName[STR_LEN] = "NULL";

	// file pointers used for reading and writing
	FILE* inputStream = NULL;
	FILE* outputStream = NULL;

	// from stdin and stdout
	if (strcmp(inputFileName,noName) == 0 && strcmp(outputFileName,noName) == 0)
	{
		inputStream = stdin;
		outputStream = stdout;
		
		// if the -srec flag is present, run in srecMode
		if (srecSwitch == 1)
		{
			srecMode(inputStream, outputStream);
		}

		// else, run in asm mode
		else
		{
			asmMode(inputStream, outputStream);
		}

	}

	// from stdin to desired output file
	if (strcmp(inputFileName,noName) == 0 && strcmp(outputFileName,noName) != 0)
	{
		inputStream = stdin;
		outputStream = fopen(outputFileName, "wb");

		// check if there is an error opening the file for writing
		if (outputStream == NULL)
		{
			printf("Error opening file for writing\n");
			return ERROR;
		}

		if (srecSwitch ==1)
		{
			srecMode(inputStream,outputStream);
		}
		else
		{
			asmMode(inputStream,outputStream);
		}
	}

	// from desired file to same output file name
	if (strcmp(inputFileName,noName) != 0 && strcmp(outputFileName,noName) == 0)
	{
		char srecExtension[STR_LEN] = ".srec";
		char asmExtension[STR_LEN] = ".asm";
		char outputFileComplete[STR_LEN] = "";

		inputStream = fopen(inputFileName, "rb");

		if (inputStream == NULL)
		{
			printf("Error opening file...\n");
			return ERROR;
		}

		strcpy(outputFileComplete, inputFileName);

		if (srecSwitch == 1)
		{
			strcat(outputFileComplete, srecExtension);
			outputStream = fopen(outputFileComplete, "wb");

			if (outputStream == NULL)
			{
				printf("Error opening file...\n");
				return ERROR;
			}
			srecMode(inputStream,outputStream);
		}

		else
		{
			strcat(outputFileComplete, asmExtension);
			outputStream = fopen(outputFileComplete, "wb");

			if (outputStream == NULL)
			{
				printf("Error opening file...\n");
				return ERROR;
			}

			asmMode(inputStream,outputStream);
		}
	}

	// from desired file input to desired file output
	if (strcmp(inputFileName,noName) != 0 && strcmp(outputFileName,noName) != 0)
	{
		inputStream = fopen(inputFileName, "rb");
		if (inputStream == NULL)
		{
			printf("Error opening file for reading...\n");
			return ERROR;
		}

		outputStream = fopen(outputFileName, "wb");
		if (outputStream == NULL)
		{
			printf("Error opening file for writing...\n");
			return ERROR;
		}

		if (srecSwitch == 1)
		{
			srecMode(inputStream,outputStream);
		}
		else
		{
			asmMode(inputStream, outputStream);
		}
	}

	if (fclose(inputStream) != 0)
	{
		printf("Error closing input file.\n");
		return ERROR;
	}

	if (fclose(outputStream) !=0)
	{
		printf("Error closing output file.\n");
		return ERROR;
	}

	return 0;
}

int showHelp(void) {
	printf("-------- Usage --------\n");
	printf("Options:\n");
	printf("-i<input_file>\tSpecify input file\n");
	printf("-o<output_file>\tSpecify output file\n");
	printf("-srec\t\tConvert data into an S-Record file\n");
	printf("-h\t\tShow this help message\n");

	return 0;
}
