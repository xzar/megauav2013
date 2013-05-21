#include "muav_com.h"
#include <stdio.h>

void initMuavCom( MuavCom *mc )
{
	mc->mc_fid=0;
	mc->mc_did=0;
	mc->mc_request=0;
	mc->mc_timestamp=0;
	mc->mc_error=0;
	
	mc->mc_dataSize=0;
	mc->mc_remainSize=0;
	
	int i;
	for (i = 0; i < SIZE; i++) 
	{
		mc->mc_data[i] = 0;
		mc->mc_remainingData[i] = 0;
	}
	
	mc->mc_headerSize = ( sizeof(int) * 4 ) + sizeof(long);
}

void setHeader( MuavCom *mc, int fid, int did, RequestType rt, long timestamp, int error )
{
	mc->mc_fid=fid;
	mc->mc_did=did;
	mc->mc_request=rt;
	mc->mc_timestamp=timestamp;
	mc->mc_error=error;
	
}

/*
 * convert header info in byte*
 */ 
void MCEncode( MuavCom *mc )
{
	int intSize = sizeof(int);
	int longSize = sizeof(long);
	
	byte fid[intSize];
	byte did[intSize];
	byte rt[intSize];
	byte timestamp[longSize];
	byte error[intSize];
	
	//int debug;
	
	convertIntToByte(mc->mc_fid, fid, intSize);
	convertIntToByte(mc->mc_did, did, intSize);
	convertIntToByte(mc->mc_request, rt, intSize);
	//printf("\nDEBUG: %d\n", sizeof(mc->mc_request));
	convertLongToByte(mc->mc_timestamp, timestamp, intSize);
	//printf("\nDEBUG: %d\n", sizeof(mc->mc_timestamp));
	convertIntToByte(mc->mc_error, error, intSize);
	
	//printf("\nDEBUG:%d\n", debug);
	
	concatBytes(mc->mc_data, mc->mc_dataSize, fid, intSize);
	mc->mc_dataSize+=intSize;
	
	concatBytes(mc->mc_data, mc->mc_dataSize, did, intSize);
	mc->mc_dataSize+=intSize;
	
	concatBytes(mc->mc_data, mc->mc_dataSize, rt, intSize);
	mc->mc_dataSize+=intSize;
	
	concatBytes(mc->mc_data, mc->mc_dataSize, timestamp, intSize);
	mc->mc_dataSize+=intSize;
	
	concatBytes(mc->mc_data, mc->mc_dataSize, error, intSize);
	mc->mc_dataSize+=intSize;
}

/*
 * convert byte* data in header info
 */ 
void MCDecode( MuavCom *mc )
{
	int intSize = sizeof(int);
	int longSize = sizeof(long);
	int index = 0;
	
	mc->mc_fid=convertByteToInt(mc->mc_data, index, intSize);
	index+=intSize;
	
	mc->mc_did=convertByteToInt(mc->mc_data, index, intSize);
	index+=intSize;
	
	mc->mc_request=convertByteToInt(mc->mc_data, index, intSize);
	index+=intSize;
	
	mc->mc_timestamp=convertByteToLong(mc->mc_data, index, longSize);
	index+=longSize;
	
	mc->mc_error=convertByteToInt(mc->mc_data, index, intSize);
	
}

/*
void sendData()
{
	
}

void *receiveData(void*data)
{
	
}
*/
void printMC(MuavCom mc)
{
	printf("\n");
	printf("MuavCom:\n");
	printf("fid:%d\ndid:%d\nrt:%d\nts:%ld\nerr:%d\n", mc.mc_fid, mc.mc_did, mc.mc_request, mc.mc_timestamp, mc.mc_error);
	printf("data:%s\n",  mc.mc_data);
	printf("Remaining Data:%s\n",  mc.mc_remainingData);
}
