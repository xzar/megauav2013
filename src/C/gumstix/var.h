#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TEST 2
//#define DEBUG1
//#define DEBUG2

struct str_DebugOut
{
    unsigned char Status[2];
    signed int Analog[32];    // Debugvalue can be displayed in MK-Tool as value or graph
};

struct str_ExternControl
{
	unsigned char Digital[2];	 //two 1/0-buttons, not used atm
	unsigned char RemoteButtons; // ? something to control the lcd menues?!
	signed char   Nick;			
	signed char   Roll;
	signed char   Gier;
	unsigned char   Gas;		//gas value is limited (in the fc sw) to the gas *stick* value
	signed char   Hight;	//sets the hight for the barometric-hight-sensor
	unsigned char free;		//not used atm
	unsigned char Frame;	//value, which is send back by the fc for confirmation
	unsigned char Config;	//if set to 1 the ExternControl is set active
};
