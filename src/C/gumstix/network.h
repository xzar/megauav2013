#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#include "muav_com.h"
#include "net_fifo.h"

/*
 * GLOBAL
 */
extern NetFifo globalNetFifo;

typedef struct network
{
	char *nt_ip;
	int nt_port;
} Network;

/*
 * network thread, listen control tower
 */
void* th_receiver(void* data);

/*
 * Send data to the control tower.
 * Encoding must be done before send the data.
 */
int sendData(MuavCom mc, int port, const char *ip);
