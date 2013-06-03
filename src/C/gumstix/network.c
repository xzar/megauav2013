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
	int n, exp_len,offset,cpt ;
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
		
		memcpy(mc.mc_data, buf, BUFFER_SIZE);
		
		MCDecode(&mc);
		
		printMC(mc);
		sem_wait(&mutex_fifo);
		addNetFifo(&globalNetFifo, buf); 
		sem_post(&mutex_fifo);
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
	Network_info nt = *(Network_info*)data;
	int sock;
	struct sockaddr_in recv_addr, exp_addr ;

	int n, exp_len,offset,cpt = 0 ;
	int buf[BUFFER_SIZE];
	int parameters[1], file;
	int i=0;
	int*  AnalogData   = (int*)  malloc( ANALOG_SIZE   * sizeof(int)  );

	sock = socket(PF_INET, SOCK_DGRAM, 0) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (nt.nt_port) ;
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	
	MuavCom mc;
	
	
	

	while (1)
	{
		initMuavCom(&mc);
		

		// Resets reading offset
		offset = 0;
		
		if( cpt == 0 )
		{
			// Sets auto send interval (x * 10 => ms)
			parameters[0] = 20;

			// Requests debug packet from FC
			SendOutData('d', 'b', parameters, 1, file_mkusb);

			cpt = 50;
		}
		cpt --;
		// Reads the first packet from the FC
		offset += read(file_mkusb, &rx_buffer+offset, TAILLE_BUFER-offset);
		
		// Checks if it's a debug packet
		if(rx_buffer[0] == '#' && rx_buffer[2] == 'D')
		{
			
			// Reads to the end
			while( rx_buffer[offset-1] != '\r' )
			{
				offset += read(file_mkusb, &rx_buffer[offset], TAILLE_BUFER-offset);
			}
		
			// Decodes the packet
			Decode64(buf, rx_buffer, offset,3,offset);
			
			// Converts data to integer
			for (i = 0; i < ANALOG_SIZE; i++)
        	{
        		AnalogData[i] = Data2Int(buf, (i * 2) + 2);
				
        	}
			
			printf("%d %d %d %d \n",AnalogData[0],AnalogData[1],AnalogData[30],AnalogData[31]);
			setHeader( &mc, 0, 0,SEND_INFO, 0 );
			
			InfoEncode(&mc, AnalogData, ANALOG_SIZE);
			
			sendData(mc, nt.nt_port, nt.nt_ip);
			//n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
		
			//MCDecode(&mc);
			
		}
		
		
		
		
		/*
		 * TODO traiter erreur de la requete si besoin
		 * pas de timeout reponse useless
		 */
		
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
