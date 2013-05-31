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

#define MODE_MANUAL 7686
#define MODE_AUTO	2754
#define MODE_OFF	7565

/*
 * GLOBAL
 */
extern NetFifo globalNetFifo;

extern sem_t mutex_fifo;
extern sem_t mutex_status;
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
 * wait for response
 */
void *th_sendInfo(void *data);

/*
 * thread, auto mode
 *
void *self_ruling(void *data);

/*
 * thread, manual mode
 *
void *manual_ruling(void *data);
*/
#endif
