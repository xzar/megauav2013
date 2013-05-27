#include "muav_com.h"

/*
 * Set mc with 0.
 */ 
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
	for (i = 0; i < BUFFER_SIZE; i++) 
	{
		mc->mc_data[i] = 0;
		mc->mc_remainingData[i] = 0;
	}
	
	mc->mc_headerSize = ( sizeof(int) * 4 ) + sizeof(long);
}

/*
 * Set the request's header.
 * fid : the fleet id.
 * did : the drone id
 * rt : the type of this request.
 * timestamp : the request date.
 * error : the error code.
 */ 
void setHeader( MuavCom *mc, int fid, int did, RequestType rt, long timestamp, int error )
{
	mc->mc_fid=fid;
	mc->mc_did=did;
	mc->mc_request=rt;
	mc->mc_timestamp=timestamp;
	mc->mc_error=error;
	
}

/*
 * convert header info in char*
 */ 
void MCEncode( MuavCom *mc )
{
	int intSize = sizeof(int);
	int longSize = sizeof(long);
	
	char fid[intSize];
	char did[intSize];
	char rt[intSize];
	char timestamp[longSize];
	char error[intSize];
	
	//int debug;
	
	convertIntTochar(mc->mc_fid, fid, intSize);
	convertIntTochar(mc->mc_did, did, intSize);
	convertIntTochar(mc->mc_request, rt, intSize);
	//printf("\nDEBUG: %d\n", sizeof(mc->mc_request));
	convertLongTochar(mc->mc_timestamp, timestamp, intSize);
	//printf("\nDEBUG: %d\n", sizeof(mc->mc_timestamp));
	convertIntTochar(mc->mc_error, error, intSize);
	
	//printf("\nDEBUG:%d\n", debug);
	
	concatchars(mc->mc_data, mc->mc_dataSize, fid, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, did, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, rt, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, timestamp, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, error, intSize);
	mc->mc_dataSize+=intSize;
}

/*
 * convert char* data in header info
 */ 
void MCDecode( MuavCom *mc )
{
	int intSize = sizeof(int);
	int longSize = sizeof(long);
	int index = 0;
	
	mc->mc_fid=convertcharToInt(mc->mc_data, index, intSize);
	index+=intSize;
	
	mc->mc_did=convertcharToInt(mc->mc_data, index, intSize);
	index+=intSize;
	
	mc->mc_request=convertcharToInt(mc->mc_data, index, intSize);
	index+=intSize;
	
	mc->mc_timestamp=convertcharToLong(mc->mc_data, index, longSize);
	index+=longSize;
	
	mc->mc_error=convertcharToInt(mc->mc_data, index, intSize);
	
}

/*
 * display data of muavcom.
 */ 
void printMC(MuavCom mc)
{
	printf("\n");
	printf("MuavCom:\n");
	printf("fid:%d\ndid:%d\nrt:%d\nts:%ld\nerr:%d\n", mc.mc_fid, mc.mc_did, mc.mc_request, mc.mc_timestamp, mc.mc_error);
	printf("data:%s\n",  mc.mc_data);
	printf("Remaining Data:%s\n",  mc.mc_remainingData);
}
