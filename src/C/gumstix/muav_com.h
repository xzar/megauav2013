#ifndef _H_MUAV_COM_
#define _H_MUAV_COM_

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"
#include "var.h"
#include "gps.h"

#define EMERGENCY_PORT 1337
#define SEND_PORT 1338
#define HEADER_SIZE 24

typedef enum
{
	PILOTE_REQ_MANUAL=1,
	R_PILOTE_REQ_MANUAL=2,
	SEND_INFO=3,
	R_SEND_INFO=4,
	EMERGENCY=5,
	MISSION=6,
	PILOTE_MANUAL=7,
	PILOTE_REQ_AUTO=8,
	R_PILOTE_REQ_AUTO=9,
	PILOTE_REQ_OFF=10,
	R_PILOTE_REQ_OFF=11,
	SEND_IMG_SIZE=12,
	R_SEND_IMG_SIZE=13,
	SEND_IMG=14,
    SEND_GPS_INFO=15,
    HELLO=16,
    R_HELLO=17,
    R_SEND_IMG=18
} RequestType;

typedef enum
{
	ERR_GPS=1,
	ERR_MANUAL=2
} ErreurType;

typedef struct muavcom 
{
	int mc_fid;
	int mc_did;
	RequestType mc_request;
	int mc_sec;
	int mc_usec;
	int mc_error;
	char mc_data[BUFFER_SIZE];
	char mc_remainingData[BUFFER_SIZE];
	int mc_remainSize;
	int mc_dataSize;
	int mc_headerSize;
} MuavCom;

/*
 * Set mc with 0.
 */ 
void initMuavCom( MuavCom *mc );



/*
 * Set the request's header. the date is set here.
 * 
 * fid : the fleet id.
 * did : the drone id
 * rt : the type of this request.
 * error : the error code.
 */ 
void setHeader( MuavCom *mc, int fid, int did, RequestType rt, int error );

/*
 * display data of muavcom.
 */ 
void printMC(MuavCom mc);

/*
 * convert char* data in header info
 */ 
void MCDecode( MuavCom *mc );

/*
 * convert header info in char*
 */ 
void MCEncode( MuavCom *mc );

/*
 * Encode the quadri info.
 */
void InfoEncode(MuavCom *mc, int *info, int size);

/*
 * put the received value in the int*;
 */
void ManualDecode(MuavCom*,int*, int*, int*, int*);

/*
 * Encode the image size.
 */
void ImgSizeEncode(MuavCom *mc, int height, int width);

/*
 * Encode the gps info
 * message form : latitude:longitude:n_sat:altitude
 */
void GPSEncode(MuavCom *mc, GPGGA gpgga);

/*
 * encode a image
 */
int imageEncode(MuavCom *mc, unsigned char *img, int img_size, char * encodedData, int offset, char *part);

#endif
