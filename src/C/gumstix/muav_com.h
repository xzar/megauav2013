#include "tools.h"


#define SIZE 4096
#define EMERGENCY_PORT 1337
#define SEND_PORT 1338

typedef enum
{
	PILOTE_REQ_MANUAL=1,
	R_PILOTE_REQ_MANUAL=2,
	REQUEST_INFO=3,
	R_REQUEST_INFO=4,
	EMERGENCY=5,
	MISSION=6,
	PILOTE_MANUAL=7,
	R_PILOTE_MANUAL=8
} RequestType;

typedef struct muavcom 
{
	int mc_fid;
	int mc_did;
	RequestType mc_request;
	long mc_timestamp;
	int mc_error;
	int mc_dataSize;
	byte mc_data[SIZE];
	int mc_remainSize;
	byte mc_remainingData[SIZE];
	int mc_headerSize;
} MuavCom;

/*
 * Set mc with 0;
 */ 
void initMuavCom( MuavCom *mc );

/*
 * Set the request's header.
 * fid : the fleet id.
 * did : the drone id
 * rt : the type of this request.
 * timestamp : the request date.
 * error : the error code.
 */ 
void setHeader( MuavCom *mc, int fid, int did, RequestType rt, long timestamp, int error );

/*
 * display data of muavcom.
 */ 
void printMC(MuavCom mc);

void MCEncode( MuavCom *mc );
