/*
*	FILE			: main.c
*	PROJECT		: EncodeInput Util
*	PROGRAMMERS		: Sebastian Posada
				  Jonathon Gregoric
*	FIRST VERSION		: 2023-01-30
*	DESCRIPTION		: This program is a linux utility used to transform any binary file
*					  into its equivalent S-Record output file, or assembly file for use
*					  in an embedded software development environment.
*/

#include "../inc/encodeInput.h"



int main(int argc, char* argv[])
{
	// Parse command line to identify what switches are being used
	int retCode = parseCmdLine(argc, argv);
	if (retCode == ERROR)
	{
		showHelp();
	}


	return 0;
}



