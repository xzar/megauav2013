#include "tools.h"


#define size 1024

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
	int mc_request;
	long mc_timestamp;
	int mc_error;
	byte mc_data[size];
	byte mc_remainingData[size];
} MuavCom;

MuavCom mc;

void initRequest(RequestType);
void requestEncode();
void requestDecode();
void sendData();
void *receiveData(void*data);
