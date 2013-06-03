#ifndef _H_FC_COM_
#define _H_FC_COM_

#include "serial_util.h"
#define TAILLE_BUFER 500

#define ANALOG_SIZE	         	32
#define VERSION_SIZE 		        10

// bitmask for HardwareError[0]
#define FC_ERROR0_GYRO_NICK             0x01
#define FC_ERROR0_GYRO_ROLL             0x02
#define FC_ERROR0_GYRO_YAW              0x04
#define FC_ERROR0_ACC_NICK              0x08
#define FC_ERROR0_ACC_ROLL              0x10
#define FC_ERROR0_ACC_TOP               0x20
#define FC_ERROR0_PRESSURE              0x40
#define FC_ERROR0_CAREFREE              0x80

// bitmask for HardwareError[1]
#define FC_ERROR1_I2C                   0x01
#define FC_ERROR1_BL_MISSING            0x02
#define FC_ERROR1_SPI_RX                0x04
#define FC_ERROR1_PPM                   0x08
#define FC_ERROR1_MIXER                 0x10
#define FC_ERROR1_RES1                  0x20
#define FC_ERROR1_RES2                  0x40
#define FC_ERROR1_RES3                  0x80

char tx_buffer[TAILLE_BUFER];
char rx_buffer[TAILLE_BUFER];

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

struct str_VersionInfo
{
	unsigned char SWMajor;
	unsigned char SWMinor;
	unsigned char ProtoMajor;
	unsigned char ProtoMinor;
	unsigned char SWPatch;
	unsigned char HardwareError[5];
};

void Decode64(int *ptrOut, char* RxdBuffer, unsigned char len, unsigned char ptrIn,unsigned char max);

int SendOutData(unsigned char cmd,unsigned char addr, unsigned char *snd, unsigned char len, int file);

void AddCRC(unsigned int frame_length);
#endif
