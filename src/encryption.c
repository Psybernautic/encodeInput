/*
*	FILE			: encryption.c
*	PROJECT		: EncodeInput Util
*	PROGRAMMERS		: Sebastian Posada 
				  Jonathon Gregoric
*	FIRST VERSION		: 2023-01-30
*	DESCRIPTION		: This file contains the core functions that deal
*				  with encoding input and output for SREC and ASSEMBLY
*			          format
*/
#include "../inc/encodeInput.h"

int srecMode(FILE* inputStream, FILE* outputStream)
{
	char myName[STR_LEN] = "SEBASTIAN-JONATH";

	// S-Record types
	// S0 , S1 , S5 , S9
	char type[4][3] = {
		"S0", "S1", "S5", "S9"
	};

	char buffer[LINE_SIZE + sizeof(char)] = "";

	int readByte = 0;
	
	int S0 = 0;
	int S1 = 0;
	int AAAAS1 = 0;

	// Parse the input from the file stream passed
	while (readByte = fread(buffer, BYTE_SIZE, LINE_SIZE, inputStream))
	{
		// write the S0 line
		if (S0 == 0)
		{
			int CCS0 = ADDRESS_SIZE + strlen(myName) + BYTE_SIZE;
			int AAAAS0 = 0;

			// TT CC AAAA
			fprintf(outputStream, "%s%02X%04X", type[0], CCS0, AAAAS0);

			int sumDataField = 0;
			for (int i = 0; i < strlen(myName); i++)
			{
				fprintf(outputStream, "%02X", sumDataField);
				sumDataField += myName[i];
			}
			

			// MM
			int MMS0 = checkSUM(CCS0, AAAAS0, sumDataField);
			fprintf(outputStream, "%02X\n", MMS0);

			S0++;
		}

		// writing the S1 line
		int CCS1 = ADDRESS_SIZE + readByte + BYTE_SIZE;

		int sumData = 0;

		// TT CC AAAA
		fprintf(outputStream, "%s%02X%04X", type[1], CCS1, AAAAS1);

		// read through the buffer one byte at a time
		for (int i = 0; i < readByte; i++)
		{
			// DD..DD
			fprintf(outputStream, "%02X", buffer[i]);
			sumData += buffer[i];
		}

		int MMS1 = checkSUM(CCS1, AAAAS1, sumData);

		fprintf(outputStream, "%02X\n", MMS1);

		AAAAS1 += LINE_SIZE;
		S1++;
	}


		// writing the S5 line

		int CCS5 = ADDRESS_SIZE + 0 + BYTE_SIZE;
		int AAAAS5 = S1;

		// TT CC AAAA
		fprintf(outputStream, "%s%02X%04X", type[2], CCS5, AAAAS5);
		int MMS5 = checkSUM(CCS5, AAAAS5, 0);

		// MM
		fprintf(outputStream,"%02X\n", MMS5);

		// writing the S9 line

		int CCS9 = ADDRESS_SIZE + 0 + BYTE_SIZE;
		int AAAAS9 = 0;

		// TT CC AAAA
		fprintf(outputStream, "%s%02X%04X", type[3], CCS9, AAAAS9);
		int MMS9 = checkSUM(CCS9, AAAAS9, 0);

		fprintf(outputStream, "%02X\n", MMS9);


	

	return 0;
}

int asmMode(FILE* inputStream, FILE* outputStream)
{
	char buffer[LINE_SIZE + sizeof(char)] = "";

	// check if we are at the end of the file and read the bytes
	int readByte = 0;
	while (readByte = fread(buffer, BYTE_SIZE, LINE_SIZE, inputStream))
	{
		// dc.b is used as a define in assmebly
		char constantByte[5] = "dc.b";
		fprintf(outputStream, "%s\t\t", constantByte);

		for (int i = 0; i < readByte; i++)
		{
			if (i == readByte -1)
			{
				fprintf(outputStream, "$%02X\n", buffer[i]);
			}

			else
			{
				fprintf(outputStream, "$%02X", buffer[i]);
			}
		}


	}
}

int checkSUM(int count, int address, int sumDataField)
	{
		int retValue = count + address + sumDataField;

		// AND to get the 1's complement
		retValue = (~retValue) & 0xFF;

		return retValue;
	}
