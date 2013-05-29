#ifndef _H_FC_COM_
#define _H_FC_COM_

#include "serial_util.h"

char tx_buffer[150];
char rx_buffer[150];


void Decode64(unsigned char *ptrOut, unsigned char len, unsigned char ptrIn,unsigned char max);

void SendOutData(unsigned char cmd,unsigned char addr, unsigned char *snd, unsigned char len, int file);

void AddCRC(unsigned int frame_length);
#endif
