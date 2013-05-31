//#include <curses.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "pilotage.h"
#include "network.h"

int main(int argc, char *argv[]) {
	
	//tower control info
	char * ip_tower;
	
	//send the drone info with this port
	int port_send;
	
	//listen the control tower in this port
	int port_listen;
	
	/*
	 * ARG MANAGMENT
	 */
	 
	if (argc != 4)
	{
		fprintf(stderr, "Invalid number of argument : %d\n", argc);
		exit(0);
	}
	
	gethostbyname(argv[1]);
	
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
	
	ip_tower = argv[1];
	
	/*
	 * END ARG MANAGMENT
	 */
	 
	Network net_listen, net_send;
	
	net_listen.nt_ip = gethostbyname(ip_tower);
	net_listen.nt_port = port_listen;
	
	net_send.nt_ip = gethostbyname(ip_tower);
	net_send.nt_port = port_send;
	
	sem_init(&mutex_fifo, NULL, 1);
	sem_init(&mutex_status, NULL, 1);
	sem_init(&sem_off, NULL, 0);
	
	status = MODE_OFF;
	
	/*
	 * THREAD
	 */
	 
	pthread_t thread_network_receiver, thread_network_sender;
	pthread_t thread_manual_ruling, thread_self_ruling;
	
	pthread_create(&thread_network_receiver, NULL, th_receiver, &net_listen);
	//pthread_create(&thread_network_sender, NULL, th_sendInfo, &net_send);
	
	//pthread_create(&thread_self_ruling,NULL,self_ruling,NULL);
	//pthread_create(&thread_manual_ruling,NULL,manual_ruling,NULL);

    /*
     * END THREAD
	 */
	
	//TODO Send coucou

    while(1)
    {
        sem_wait(&mutex_status);

        switch(status)
        {
            case MODE_MANUAL:
                sem_post(&mutex_status);
                /*
                 * CODE MODE MANUAL
                 */
                break;
            case MODE_AUTO:
                sem_post(&mutex_status);
                /*
                 * CODE MODE WITH IA
                 */
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

    
	//pthread_join(thread_network_receiver, NULL);
	//pthread_join(thread_network_sender, NULL);
	
	//pthread_join(thread_self_ruling, NULL);
	//pthread_join(thread_manual_ruling, NULL);
	 
	 
	
	

	/*
	 * TEST ALEX
	 * 
	 * 
    printf ("char : %d octets\n", sizeof(char));  
    printf ("int : %d octets\n", sizeof(int));  
    printf ("long : %d octets\n", sizeof(long));  
    printf ("double: %d octets\n", sizeof(double));

    int i=0,j=0;
    
    byte toto[2048];
    int size_toto = 10;
    byte tata[2048];
    int size_tata = 5;
    
    for( i = 0; i < size_toto;i++ ) 
    {
		toto[i]='a';
	}
	
	for( i = 0; i < size_tata;i++ ) 
    {
		tata[i]='b';
	}
	
	concatBytes(toto, size_toto, tata, size_tata);
	size_toto+=size_tata;
	
	for( i = 0; i < size_toto;i++ ) 
    {
		printf("%c\n", toto[i]);
	}
	
	cutBytes (toto, size_toto, 6);
	size_toto-=6;
	
	printf("debug\n");
	
	for( i = 0; i < size_toto;i++ ) 
    {
		printf("%c\n", toto[i]);
	}
	
	int tt = 123456789;
	byte tab[4];
	
	int test = convertIntToByte(tt, tab, 4);
	
	if (test = -1) printf("error\n");
	
	tt = convertByteToInt(tab, 0, 4);
	
	printf("%d\n", tt);

    exit(0);
    * 
    * FIN TEST ALEX
    * 
    */
    
    /*
     * TEST MUAVCOM
     *
    
    long toto = 9999999999999999;
    printf("\n%ld\n", toto);
    printf("\nsize long:%d\n", sizeof(long));
    MuavCom mc1, mc2;
    initMuavCom(&mc1);
    initMuavCom(&mc2);
    setHeader(&mc1, 11111111,22222222,1,toto,88888888);
    printMC(mc1);
    char* buf;
    receive(socket, buf, sizeof(),)
    //memcpy(&mc2, &mc1, SIZE);
    
    MCEncode(&mc1);
    //printMC(mc2);
    mc1.mc_data[mc1.mc_headerSize] = "\0";
    
    //memcpy(&mc2, mc1.mc_data, mc1.mc_headerSize);
    
    //mc2 = (MuavCom)mc1.mc_data;
    
    //memcpy(mc2.mc_data, mc1.mc_data, SIZE);
    //MCDecode(&mc2);
    
    printMC(mc2);
    
    *
    * FIN TEST MUAV COM
    */
    
    /*
     * TEST thread
     */
    
    //initNetFifo(globalNetFifo);
    
    
    /*
     * 
     * test reseau
    initNetFifo(&globalNetFifo);
    
    Network net_info;
    net_info.nt_ip = gethostbyname("localhost");
    net_info.nt_port = 5000;
    
	//pthread_t networt_thread_receiver;
	//pthread_create(&networt_thread_receiver, NULL, th_receiver, &net_info);
	
	//pthread_join(networt_thread_receiver, NULL);
	
	MuavCom mc;
	initMuavCom(&mc);
	setHeader(&mc, 11111111,22222222,1,555555,88888888);
	
	MCEncode(&mc);
	
	//printf("too\n");
	
	sendData(mc, 5000, "127.0.0.1");
	*
	* FIN TEST TEST RESAU
	*/
	
	//while(1);
	
     /* 
     * FIN TEST THREAD 
     */
    
    
    /*
	int file;
	char* buf_reseau = (char *) malloc(20*sizeof(char));
	//struct pour le control
	

	//structure pour la reception du debug
	//struct str_DebugOut DebugOut;

	//ouverture du port série
	serial_open(&file, "/dev/ttyUSB0");
	
	
	//envoi requete pour le debug
	
	//SendOutData('d', 0, buf2, 0, file);

	//init reseau
	open_reseau();



	memset(&ExternControl, 0, sizeof(struct str_ExternControl) );

	init_pilotage();

	while(1){
		
		

			attente_reseau(buf_reseau, 4);
			printf("ca passe apres\n");
			set_Nick( (signed char) buf_reseau[0] );
			set_Roll( (signed char) buf_reseau[1] );
			set_Yaw( (signed char) buf_reseau[2] ); 
			set_Gas( (unsigned char) buf_reseau[3] ); 
			
			printf("bufreseau = %s \n ",buf_reseau);
			 //envoi_pilotage(file);
	}
	
		
		
	
//	close_reseau();
	close(file);
	free(buf_reseau);
    */
    
    /*
     * 
     * 
     * REAL AND LAST MAIN !!!!!!!
     * 
     * 
     */ 
    
    return 1;
}  
