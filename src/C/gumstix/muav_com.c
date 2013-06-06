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
	
	mc->mc_headerSize = HEADER_SIZE;
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
	convertIntTochar(mc->mc_sec, sec, intSize);
	convertIntTochar(mc->mc_usec, usec, intSize);
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
	
	mc->mc_sec=convertcharToInt(mc->mc_data, index, 4);
	index+=4;
	
	mc->mc_usec=convertcharToInt(mc->mc_data, index, 4);
	index+=4;
	
	mc->mc_error=convertcharToInt(mc->mc_data, index, 4);
	//index+=4;
	
}

/*
 * display data of muavcom.
 */ 
void printMC(MuavCom mc)
{
	printf("\n");
	printf("MuavCom:\n");
	printf("fid:%d\ndid:%d\nrt:%d\nsec:%d\nusec:%d\nerr:%d\n", mc.mc_fid, mc.mc_did, mc.mc_request, mc.mc_sec, mc.mc_usec, mc.mc_error);
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
	char tmp[4];
	
	for (; i < size; i++)
	{
		convertIntTochar(info[i], tmp, 4);
		concatchars(mc->mc_data, mc->mc_dataSize, tmp, 4);
		mc->mc_dataSize+=4;
	}
}

/*
 * put the received value in the int*;
 */
void ManualDecode(MuavCom* mc, int* nick, int* roll, int* yaw, int* gas)
{
	*nick = convertcharToInt(mc->mc_data, HEADER_SIZE, 4);
	*roll = convertcharToInt(mc->mc_data, HEADER_SIZE+4, 4);
	*yaw = convertcharToInt(mc->mc_data, HEADER_SIZE+8, 4);
	*gas = convertcharToInt(mc->mc_data, HEADER_SIZE+12, 4);
}

/*
 * Encode the image size.
 */
void ImgSizeEncode(MuavCom *mc, int height, int width)
{
	MCEncode(mc);
	
	char c_height[4];
	char c_width[4];
	
	convertIntTochar(height, c_height, 4);
	convertIntTochar(width, c_width, 4);
	
	concatchars(mc->mc_data, mc->mc_dataSize, c_height, 4);
	mc->mc_dataSize+=4;
	
	concatchars(mc->mc_data, mc->mc_dataSize, c_width, 4);
	mc->mc_dataSize+=4;
}

/*
 * Encode the gps info
 * message form : latitude:longitude:n_sat:gpgga_accuracy_horizontal:altitude
 */
void GPSEncode(MuavCom *mc, GPGGA gpgga)
{
    MCEncode(mc);

    int len1, len2, len3, len4, len5;
    char buf[BUFFER_SIZE];

    len1 = strlen(gpgga.gpgga_latitude);
    len2 = strlen(gpgga.gpgga_longitude);
    len3 = strlen(gpgga.gpgga_n_sat);
    len4 = strlen(gpgga.gpgga_accuracy_horizontal);
    len5 = strlen(gpgga.gpgga_altitude);

    memcpy(buf, gpgga.gpgga_latitude, len1);
    buf[len1]=':';
    memcpy(&buf[len1+1], gpgga.gpgga_longitude, len2);
    buf[len1+len2+1]=':';
    memcpy(&buf[len1+len2+2], gpgga.gpgga_n_sat, len3);
    buf[len1+len2+len3+2]=':';
    memcpy(&buf[len1+len2+len3+3], gpgga.gpgga_accuracy_horizontal, len4);
    buf[len1+len2+len3+len4+3]=':';
    memcpy(&buf[len1+len2+len3+len4+4], gpgga.gpgga_altitude, len5);
    buf[len1+len2+len3+len4+len5+4]='\0';

    len1 = strlen(buf);
    
    mc->mc_dataSize+=len1;

    memcpy(&mc->mc_data[HEADER_SIZE], buf, len1);
}
