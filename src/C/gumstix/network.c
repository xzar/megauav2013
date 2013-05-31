#include "network.h"

/*
 * GLOBAL
 */
NetFifo globalNetFifo;

sem_t mutex_fifo;
sem_t mutex_status;
sem_t sem_off;

int status;

/*
 * Send data to the control tower.
 * Encoding must be done before send the data.
 */
int sendData(MuavCom mc, int port, const char *ip)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in dest;
	int socklen = sizeof(dest);
	struct hostent* serv = NULL;
	
	serv = gethostbyname(ip);
	
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr = *(struct in_addr*)serv->h_addr;
	
	printMC(mc);
	
	sendto(sock, mc.mc_data, mc.mc_dataSize, 0, (struct sockaddr*)&dest, (socklen_t)socklen);
	
	close(sock);
	
	return 1;
}

void* th_receiver(void* data)
{
	Network nt = *(Network*)data;
	int sock;
	struct sockaddr_in recv_addr, exp_addr ;
	int n, exp_len ;
	char buf[BUFFER_SIZE];
	
	sock = socket(PF_INET, SOCK_DGRAM, 0) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (nt.nt_port) ;
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	
	//
	
	MuavCom mc;
	
	printf("wait port : %d\n", nt.nt_port);
	
	while (1)
	{
		n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
		//TODO MUTEX
		
		printf("received: %s", buf);
		printf("packet size %d\n", exp_len);

        
		int i;
		for (i = 0; i < BUFFER_SIZE; i++) {
			mc.mc_data[i] = buf[i];
		}
		
		MCDecode(&mc);
		
		printMC(mc);
		
		addNetFifo(&globalNetFifo, buf); 
		
		//close(sock);
		
	} 
	
	close(sock);
}

/*
 * send the drone info to the control tower.
 * wait for response
 */
void *th_sendInfo(void *data)
{
	Network nt = *(Network*)data;
	int sock;
	struct sockaddr_in recv_addr, exp_addr ;
	int n, exp_len ;
	char buf[BUFFER_SIZE];
	
	sock = socket(PF_INET, SOCK_DGRAM, 0) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (nt.nt_port) ;
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	
	MuavCom mc;
	
	int i = 0;
	while (1)
	{
		/*
		 * TODO ENVOI INFO
		 */
		//sendData(mc, nt.nt_port, nt.nt_ip);
		
		//n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
		
		//MCDecode(&mc);
		
		/*
		 * TODO traiter erreur de la requete si besoin
		 * pas de timeout reponse useless
		 */
		//printf("DEBUG1\n");
		//sleep(1);
		
		
		/*
		if (i < 3)
		{
			//sem_trywait(&sem_manual);
			sem_post(&sem_auto);
		} else {
			//sem_trywait(&sem_auto);
			sem_post(&sem_manual);
		}
		*/
		//i++;
		//printf("DEBUG2\n");
	} 
	
	close(sock);
}

/*
 * thread, auto mode
 *
void *self_ruling(void *data)
{
	while (1)
	{
		sem_wait(&sem_auto);
		
		printf("test auto\n");
		
		sem_post(&sem_sync);
		usleep(500000);
	}
}

/*
 * thread, manual mode
 *
void *manual_ruling(void *data)
{
	while (1)
	{
		sem_wait(&sem_manual);
		
		printf("test manual\n");
		
		sem_post(&sem_sync);
		usleep(500000);
	}
}


/*
 * Sync manual and auto mode
 *
 void * sync_mode(void *data)
 {
	 while (1)
	 {
		 sem_wait(&sem_sync);
		 switch(status)
		 {
			 case MODE_AUTO:
				sem_post(&sem_auto);
				break;
			 case MODE_MANUAL:
				sem_post(&sem_manual);
				break;
			 case MODE_OFF:
				break;
		 }
	 }
 }
*/
