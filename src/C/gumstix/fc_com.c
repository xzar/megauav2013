#include "fc_com.h"



void AddCRC(unsigned int frame_length) 
{ //length of #,adr,cmd,data
#ifdef DEBUG2
printf("entrer AddCRC fc_com.c\n");	
#endif
	printf("1   %s \n", tx_buffer);
	unsigned int tmpCRC = 0;
	unsigned int i;

	for (i=0; i < frame_length;i++)
	{
		tmpCRC += tx_buffer[i];
	}

	tmpCRC %= 4096;
	tx_buffer[i++] = '=' + tmpCRC / 64;
	tx_buffer[i++] = '=' + tmpCRC % 64;
	tx_buffer[i++] = '\r';

}

void Decode64(unsigned char *ptrOut, unsigned char len, unsigned char ptrIn,unsigned char max) 
{
#ifdef DEBUG2
printf("entrer Decode64 fc_com.c\n");	
#endif

	unsigned char a,b,c,d;
	unsigned char ptr = 0;
	unsigned char x,y,z;

	while(len)
	{
		a = rx_buffer[ptrIn++] - '=';
		b = rx_buffer[ptrIn++] - '=';
		c = rx_buffer[ptrIn++] - '=';
		d = rx_buffer[ptrIn++] - '=';

		if(ptrIn > max - 2) break;     // dont process more data than recieved
		
		x = (a << 2) | (b >> 4);
		y = ((b & 0x0f) << 4) | (c >> 2);
		z = ((c & 0x03) << 6) | d;
		
		if(len--) ptrOut[ptr++] = x; else break;
		if(len--) ptrOut[ptr++] = y; else break;
		if(len--) ptrOut[ptr++] = z; else break;
	}
}


void SendOutData(unsigned char cmd,unsigned char addr, unsigned char *snd, unsigned char len, int file) 
{
#ifdef DEBUG2
printf("entrer SendOutData fc_com.c\n");	
#endif

	unsigned int pt = 0;
	unsigned char a,b,c;
	unsigned char ptr = 0;

	tx_buffer[pt++] = '#';               // Start-Byte
	tx_buffer[pt++] = 'a' + addr;        // Adress
	tx_buffer[pt++] = cmd;               // Command
	while(len)
	{
		if(len) { a = snd[ptr++]; len--;} else a = 0;
		if(len) { b = snd[ptr++]; len--;} else b = 0;
		if(len) { c = snd[ptr++]; len--;} else c = 0;
		tx_buffer[pt++] = '=' + (a >> 2);
		tx_buffer[pt++] = '=' + (((a & 0x03) << 4) | ((b & 0xf0) >> 4));
		tx_buffer[pt++] = '=' + (((b & 0x0f) << 2) | ((c & 0xc0) >> 6));
		tx_buffer[pt++] = '=' + ( c & 0x3f);
	}

	AddCRC(pt);
	printf("2    %s \n", tx_buffer);
	serial_write(file, tx_buffer, pt+3)
	; //whole frame length is pt+3
}									  //#,adr,cmd,data ; crc1,crc2,\r

