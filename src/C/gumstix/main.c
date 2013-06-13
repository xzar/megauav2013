//#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>

#include "pilotage.h"
#include "network.h"
#include "serial_util.h"
#include "odile.h"

int paramIA;

void traiter_signal(){
	printf("ctrl+c trapper\n");
	if(capture != NULL)cvReleaseCapture( &capture );

	exit(0);
}
int main(int argc, char *argv[]) 
{

	paramIA =3;
	//tower control info
	char * ip_tower;
	
	//send the drone info with this port
	int port_send;
	
	//listen the control tower in this port
	int port_listen;

	AnalogData   = (int*)  malloc( ANALOG_SIZE   * sizeof(int)  );
	/*
	 * ARG MANAGMENT
	 */
	 
	if (argc != 5)
	{
		fprintf(stderr, "Invalid number of argument : %d\n", argc);
		exit(0);
	}
	
	gethostbyname(argv[1]);
	ip_tower = argv[1];
	switch(h_errno)
	{
		case HOST_NOT_FOUND:
			fprintf(stderr, "IP_ERROR : host not found\n");
			exit(0);
			break;
		case NO_DATA:
			fprintf(stderr, "IP_ERROR : no data\n");
			exit(0);
			break;
		case NO_RECOVERY:
			fprintf(stderr, "IP_ERROR : no recovery\n");
			exit(0);
			break;
		case TRY_AGAIN:
			fprintf(stderr, "IP_ERROR : try again\n");
			exit(0);
			break;
	}
	
	port_listen = atoi(argv[2]);
	
	port_send = atoi(argv[3]);
	
	/*Trappage du ctrl+c*/
	signal(SIGINT, traiter_signal);

	if ( port_send <= 0 || port_send >= 255*255 )
	{
		fprintf(stderr, "port %d is not allowed\n", port_send);
	}
	
	if ( port_listen <= 0 || port_listen >= 255*255 )
	{
		fprintf(stderr, "port %d is not allowed\n", port_listen);
	}
	
	if(serial_open(&file_mkusb, argv[4], B57600)==-1)
	{
		printf("Can't open serial port !\n");
		return -1;
	}


	ip_tower = argv[1];
	
	/*
	 * END ARG MANAGMENT
	 */
	 
	Network net_listen;
	Network_info net_info;
	int can_start = 0;
	
	net_listen.nt_ip = ip_tower;
	net_listen.nt_port = port_listen;
	net_listen.nt_port2 = port_send;
	
	net_info.nt_ip = ip_tower;
	net_info.nt_port = port_send;
	
	sem_init(&mutex_fifo, 0, 1);
	sem_init(&mutex_status, 0, 1);
	sem_init(&mutex_analog, 0, 1);
	sem_init(&sem_off, 0, 0);
	
	initNetFifo(&globalNetFifo);
	
	status = MODE_OFF;
	status_gps=0;
	status_sdimg=0;
	
	/*
	 * HELLO
	 */
	int sock;
	struct sockaddr_in recv_addr, exp_addr ;
	int n, exp_len,cpt=0 ;
	char buf[BUFFER_SIZE];
	struct timeval timeout;
	MuavCom mc1, mc2;
	
	initMuavCom(&mc1);
	initMuavCom(&mc2);
	
	timeout.tv_sec=0;
	timeout.tv_usec=TIMEOUT_MS;
	
	sock = socket(PF_INET, SOCK_DGRAM, 0) ;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (net_info.nt_port) ;
	
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	
	setHeader(&mc1, 0,0, HELLO,0);
	MCEncode(&mc1);
	 
	while ( can_start == 0 )
	{
		memset(buf,0,BUFFER_SIZE);
		sendData(mc1, net_info.nt_port, ip_tower);
		n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
		
		if (n != -1)
		{
			memcpy(mc2.mc_data, buf, BUFFER_SIZE);
			MCDecode(&mc2);
			if (mc2.mc_request == R_HELLO) can_start = 1; 
		} else {
			cpt++;
		}
		
		if (cpt >= 2)
		{
			printf("networt is shit\n");
			exit(0);
		}
	}
	
	close(sock);
	/*
	 * FIN HELLO
	 */
	
	/*
	 * THREAD
	 */
	 
	pthread_t thread_network_receiver, thread_network_sender;
	
	pthread_create(&thread_network_receiver, NULL, th_receiver, &net_listen);
	pthread_create(&thread_network_sender, NULL, th_sendInfo, &net_info);



    /*
     * END THREAD
	 */
	
	memset(&ExternControl, 0, sizeof(struct str_ExternControl) );
	init_pilotage();
	MuavCom mc;
	int iaType;
    while(1)
    {
		initMuavCom(&mc);
		
        sem_wait(&mutex_status);
        switch(status)
        {
            case MODE_MANUAL:
                sem_post(&mutex_status);
				
				int nick, roll, yaw, gas;
				
				sem_wait(&mutex_fifo);
				if ( ! isEmptyNetFifo(&globalNetFifo) )
				{
					memset(mc.mc_data, 0, BUFFER_SIZE);
					memcpy(mc.mc_data, firstNetFifo(&globalNetFifo), BUFFER_SIZE);
					removeNetFifo(&globalNetFifo);
					sem_post(&mutex_fifo);
					MCDecode(&mc);
					if (mc.mc_request == PILOTE_MANUAL)
					{
						ManualDecode(&mc, &nick, &roll, &yaw, &gas);
#ifdef DEBUGJOY1
printf("commande moteur : Nick = %d, Roll = %d, Yaw = %d, Gas = %d",nick,roll,yaw,gas);
#endif
						set_Nick( (signed char) nick );
						set_Roll( (signed char) roll );
						set_Yaw( (signed char) yaw ); 
						set_Gas( (unsigned char) gas );
						envoi_pilotage(file_mkusb);

					}
				} else {
					sem_post(&mutex_fifo);
				}
				
                break;
            case MODE_AUTO:
                sem_post(&mutex_status);
                sem_wait(&mutex_fifo);
                if ( ! isEmptyNetFifo(&globalNetFifo) )
				{
					memset(mc.mc_data, 0, BUFFER_SIZE);
					memcpy(mc.mc_data, firstNetFifo(&globalNetFifo), BUFFER_SIZE);
					removeNetFifo(&globalNetFifo);
					sem_post(&mutex_fifo);
					iaType = decodeIA(&mc);
					if (mc.mc_request == PILOTE_REQ_AUTO)
					{
						
//#ifdef DEBUGJOY1
printf("type ia : %d\n", iaType);
//#endif
						if ( iaType == 1 ){
							deplacement_zero();
						}else if ( iaType == 2 )
						{
							converge();
						}else if ( iaType == 3 ){
							prise_photo();
						}
					}
				} else {
					sem_post(&mutex_fifo);
				}
                break;
            case MODE_OFF:
                sem_post(&mutex_status);
                sem_wait(&sem_off);
                break;
        }
    }

    /*
     * END
     */
    return 1;
}  
