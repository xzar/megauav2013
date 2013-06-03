#include "fc_com.h"



void AddCRC(unsigned int frame_length) 
{ //length of #,adr,cmd,data
#ifdef DEBUG2
printf("entrer AddCRC fc_com.c\n");	
#endif
	#ifdef DEBUG1
	printf("TX buffer add CRC pilota.C   %s \n", tx_buffer);
	#endif
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


int Data2Int(int *Data , int Start)
{
    int Out = (Data[Start+1]<<8) | (Data[Start+0]);

    if (Out > 32767)
      Out = Out - 65536;

    return Out;

}

int Data2Char(int *Data , int Start)
{
    int Out = (Data[Start]);

    //if ((Out > 128))
    //  Out = Out - 256;

    return Out;

}

void Decode64(int *ptrOut, char* RxdBuffer, unsigned char len, unsigned char ptrIn,unsigned char max)
{
	unsigned char a,b,c,d;
	unsigned char ptr = 0;
	unsigned char x,y,z;

	while(len != 0)
	{
		a = RxdBuffer[ptrIn++] - '=';
		b = RxdBuffer[ptrIn++] - '=';
		c = RxdBuffer[ptrIn++] - '=';
		d = RxdBuffer[ptrIn++] - '=';
	
		// dont process more data than recieved
		if(ptrIn > max - 2) break;
	
		x = (a << 2) | (b >> 4);
		y = ((b & 0x0f) << 4) | (c >> 2);
		z = ((c & 0x03) << 6) | d;
	
		if(len--) ptrOut[ptr++] = x; else break;
		if(len--) ptrOut[ptr++] = y; else break;
        	if(len--) ptrOut[ptr++] = z; else break;
       }
}

int SendOutData(unsigned char cmd,unsigned char addr, unsigned char *snd, unsigned char len, int file) 
{
#ifdef DEBUG2
printf("entrer SendOutData fc_com.c\n");	
#endif

	unsigned int pt = 0;
	unsigned char a,b,c;
	unsigned char ptr = 0;

	tx_buffer[pt++] = '#';               // Start-Byte
	//tx_buffer[pt++] = 'b';        // Adress
    tx_buffer[pt++] = 'b';
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
	#ifdef DEBUG1
	printf("TX Buffer    %s \n", tx_buffer);
	#endif
	return serial_write(file, tx_buffer, pt+3); //whole frame length is pt+3
}									  //#,adr,cmd,data ; crc1,crc2,\r

