#ifndef _H_NETWORK_
#define _H_NETWORK_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <semaphore.h>

#include "muav_com.h"
#include "net_fifo.h"
#include "fc_com.h"
#include "gps.h"

#define MODE_MANUAL 7686
#define MODE_AUTO	2754
#define MODE_OFF	7565

#define TIMEOUT_MS	200000

/*
 * GLOBAL
 */
extern NetFifo globalNetFifo;

extern sem_t mutex_fifo;
extern sem_t mutex_status;
extern sem_t mutex_analog;
extern sem_t sem_off;

extern int status;

/*
 * Structure Network
 */
typedef struct network
{
	char *nt_ip;
	int nt_port;
} Network;

typedef struct network_info
{
	char *nt_ip;
	int nt_port;
	char *nameFile;
} Network_info;

/*
 * network thread, listen control tower
 * typically the mode, mission, etc...
 */
void* th_receiver(void* data);

/*
 * Send the MuavCom mc packet to the ip, on the port.
 * Encoding must be done before send the data.
 */
int sendData(MuavCom mc, int port, const char *ip);

/*
 * send the drone info to the control tower.
 * 
 */
void *th_sendInfo(void *data);

/*
 * Send image to the control tower.
 */
int sendImage(int port, const char *ip, char * imgRGB, int height, int width);

/*
 * send the gps drone info to the control tower.
 * 
 */
void *th_sendGPS(void *data);

#endif
