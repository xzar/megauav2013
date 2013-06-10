//#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

#include "pilotage.h"
#include "network.h"
#include "serial_util.h"
#include "odile.h"


int main(int argc, char *argv[]) 
{
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
	
	net_listen.nt_ip = ip_tower;
	net_listen.nt_port = port_listen;
	
	net_info.nt_ip = ip_tower;
	net_info.nt_port = port_send;
	
	sem_init(&mutex_fifo, NULL, 1);
	sem_init(&mutex_status, NULL, 1);
	sem_init(&mutex_analog, NULL, 1);
	sem_init(&sem_off, NULL, 0);
	
	initNetFifo(&globalNetFifo);
	
	status = MODE_OFF;
	
	/*
	 * THREAD
	 */
	 
	pthread_t thread_network_receiver, thread_network_sender;
	pthread_t thread_manual_ruling, thread_self_ruling;
	
	pthread_create(&thread_network_receiver, NULL, th_receiver, &net_listen);
	pthread_create(&thread_network_sender, NULL, th_sendInfo, &net_info);
	
	//pthread_create(&thread_self_ruling,NULL,self_ruling,NULL);
	//pthread_create(&thread_manual_ruling,NULL,manual_ruling,NULL);

    /*
     * END THREAD
	 */
	
	//pthread_join(thread_network_receiver, NULL);
	//pthread_join(thread_network_sender, NULL);
	
	//TODO Send coucou
	
	memset(&ExternControl, 0, sizeof(struct str_ExternControl) );
	init_pilotage();
//printf("test 0\n");
    while(1)
    {
        sem_wait(&mutex_status);
//printf("test 01\n");
        switch(status)
        {
            case MODE_MANUAL:
                sem_post(&mutex_status);
				//printf("test 1 \n");
				
				int nick, roll, yaw, gas;
				MuavCom mc;
				//printf("test 2\n");
				sem_wait(&mutex_fifo);
				//printf("test 3\n");
				if ( ! isEmptyNetFifo(&globalNetFifo) )
				{
					//printf("test 4444\n");
					memset(mc.mc_data, 0, BUFFER_SIZE);
					//printf("test 445\n");
					memcpy(mc.mc_data, firstNetFifo(&globalNetFifo), BUFFER_SIZE);
					//printf("test 55\n");
					removeNetFifo(&globalNetFifo);
					//printf("test 5\n");
					sem_post(&mutex_fifo);
					//printf("test 6\n");
					MCDecode(&mc);
					//printf("test 7\n");
					if (mc.mc_request == PILOTE_MANUAL)
					{
						//printf("test 8\n");
						ManualDecode(&mc, &nick, &roll, &yaw, &gas);
						printf("%d %d %d %d \n",nick, roll, yaw, gas);
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
                take_off( 1 );
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
