#include "muav_com.h"

/*
 * Set mc with 0.
 */ 
void initMuavCom( MuavCom *mc )
{
	mc->mc_fid=0;
	mc->mc_did=0;
	mc->mc_request=0;
	mc->mc_sec=0;
	mc->mc_usec=0;
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
 * Set the request's header. the date is set here.
 * 
 * fid : the fleet id.
 * did : the drone id
 * rt : the type of this request.
 * error : the error code.
 */ 
void setHeader( MuavCom *mc, int fid, int did, RequestType rt, int error )
{
	struct timeval tv;
	
	mc->mc_fid=fid;
	mc->mc_did=did;
	mc->mc_request=rt;
	mc->mc_error=error;
	
	gettimeofday(&tv, NULL);
	
	mc->mc_sec=tv.tv_sec;
	mc->mc_usec=tv.tv_usec;
	
}

/*
 * convert header info in char*
 */ 
void MCEncode( MuavCom *mc )
{
	int intSize = sizeof(int);
	
	char fid[intSize];
	char did[intSize];
	char rt[intSize];
	char sec[intSize];
	char usec[intSize];
	char error[intSize];
	
	convertIntTochar(mc->mc_fid, fid, intSize);
	convertIntTochar(mc->mc_did, did, intSize);
	convertIntTochar(mc->mc_request, rt, intSize);
	convertLongTochar(mc->mc_sec, sec, intSize);
	convertLongTochar(mc->mc_usec, usec, intSize);
	convertIntTochar(mc->mc_error, error, intSize);
	
	concatchars(mc->mc_data, mc->mc_dataSize, fid, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, did, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, rt, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, sec, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, usec, intSize);
	mc->mc_dataSize+=intSize;
	
	concatchars(mc->mc_data, mc->mc_dataSize, error, intSize);
	mc->mc_dataSize+=intSize;
}

/*
 * convert char* data in header info
 */ 
void MCDecode( MuavCom *mc )
{
	int index = 0;
	
	mc->mc_fid=convertcharToInt(mc->mc_data, index, 4);
	index+=4;
	
	mc->mc_did=convertcharToInt(mc->mc_data, index, 4);
	index+=4;
	
	mc->mc_request=convertcharToInt(mc->mc_data, index, 4);
	index+=4;
	
	mc->mc_sec=convertcharToLong(mc->mc_data, index, 4);
	index+=4;
	
	mc->mc_usec=convertcharToLong(mc->mc_data, index, 4);
	index+=4;
	
	mc->mc_error=convertcharToInt(mc->mc_data, index, 4);
	
}

/*
 * display data of muavcom.
 */ 
void printMC(MuavCom mc)
{
	printf("\n");
	printf("MuavCom:\n");
	printf("fid:%d\ndid:%d\nrt:%d\nts:%ld\nerr:%d\n", mc.mc_fid, mc.mc_did, mc.mc_request, mc.mc_sec, mc.mc_error);
	printf("data:%s\n",  mc.mc_data);
	printf("Remaining Data:%s\n",  mc.mc_remainingData);
}

/*
 * Encode the quadri info.
 */
void InfoEncode(MuavCom *mc, int *info, int size)
{
	MCEncode(mc);
	
	int i=0;
	int j=0;
	char tmp[4];
	
	for (; i < size; i++)
	{
		convertIntTochar(info[i], tmp, 4);
		concatchars(mc->mc_data, mc->mc_dataSize, tmp, 4);
		mc->mc_dataSize+=4;
	}
}
