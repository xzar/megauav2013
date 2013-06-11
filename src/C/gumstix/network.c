#include "network.h"

/*
 * GLOBAL
 */
NetFifo globalNetFifo;

sem_t mutex_fifo;
sem_t mutex_status;
sem_t sem_off;
sem_t mutex_analog;

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
	sendto(sock, mc.mc_data, mc.mc_dataSize, 0, (struct sockaddr*)&dest, (socklen_t)socklen);
	close(sock);
	
	return 1;
}

/*
 * Send image to the control tower.
 */
int sendImage(int port, const char *ip, char * imgRGB, int height, int width)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in dest;
	int socklen = sizeof(dest);
	struct hostent* serv = NULL;
	int size = height * width * 3;
	
	serv = gethostbyname(ip);
	dest.sin_family = AF_INET;
	dest.sin_port = htons(port);
	dest.sin_addr = *(struct in_addr*)serv->h_addr;
	sendto(sock, imgRGB, size, 0, (struct sockaddr*)&dest, (socklen_t)socklen);
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
	
	MuavCom mc;
	
	printf("receive command port : %d\n", nt.nt_port);
	
	while (1)
	{
		memset(buf,0,BUFFER_SIZE);
		n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
		
		//printf("received: %s\n", buf);
		
		memcpy(mc.mc_data, buf, BUFFER_SIZE);
		
		MCDecode(&mc);
		
		switch(mc.mc_request)
		{
			case PILOTE_REQ_MANUAL:
				sem_wait(&mutex_fifo);
				clearNetFifo(&globalNetFifo);
				sem_post(&mutex_fifo);
				
				//printf("debug 03\n");
				
				sem_wait(&mutex_status);
				status = MODE_MANUAL;
				sem_post(&mutex_status);
				
				initMuavCom(&mc);
				setHeader(&mc, 0, 0, R_PILOTE_REQ_MANUAL, 0);
				MCEncode(&mc);
				sendData(mc, nt.nt_port, nt.nt_ip);
				
				sem_post(&sem_off);
				break;
			case PILOTE_REQ_AUTO:
				sem_wait(&mutex_fifo);
				clearNetFifo(&globalNetFifo);
				sem_post(&mutex_fifo);
				
				//printf("debug 02\n");
				
				sem_wait(&mutex_status);
				status = MODE_AUTO;
				sem_post(&mutex_status);
				
				initMuavCom(&mc);
				setHeader(&mc, 0, 0, R_PILOTE_REQ_AUTO, 0);
				MCEncode(&mc);
				sendData(mc, nt.nt_port, nt.nt_ip);
				
				sem_post(&sem_off);
				break;
			case PILOTE_REQ_OFF:
				sem_wait(&mutex_fifo);
				clearNetFifo(&globalNetFifo);
				sem_post(&mutex_fifo);
				
				//printf("debug 01\n");
				
				sem_wait(&mutex_status);
				status = MODE_OFF;
				sem_post(&mutex_status);
				break;
			default:
				sem_wait(&mutex_fifo);
				addNetFifo(&globalNetFifo, buf);
				sem_post(&mutex_fifo);
				
				//printf("debug 04\n");
		}
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
	int parameters[1], file, handler;
	int i=0, info=0;
    GPGGA gpgga;
	

	sock = socket(PF_INET, SOCK_DGRAM, 0) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (nt.nt_port) ;
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	
	MuavCom mc;
	//serial_open(&handler, "/dev/ttyS0");
	open_gps();
    
	while (1)
	{
        memset(buf, 0, BUFFER_SIZE);
		initMuavCom(&mc);
		
		//
		
		//printf("envoi boucle\n");
		
		//
		//printf("toto \n");
		
        /*
         * one time send the microkopter info
         * next send the gps info
         */
				//printf("toto 1 \n");
      /*  if (info == 0)
        {*/

            // Resets reading offset
            offset = 0;
            
            		//printf("toto2 \n");
            if( cpt == 0 )
        	{
                // Sets auto send interval (x * 10 => ms)
                parameters[0] = 20;

                // Requests debug packet from FC
                SendOutData('d', 'b', parameters, 1, file_mkusb);

                cpt = 10;
        	}
			cpt --;
            // Reads the first packet from the FC
            offset += read(file_mkusb, &rx_buffer+offset, TAILLE_BUFER-offset);
            		//printf("toto 3\n");
            // Checks if it's a debug packet
            if(rx_buffer[0] == '#' && rx_buffer[2] == 'D')
            {
                		//printf("toto 4\n");
                // Reads to the end
                while( rx_buffer[offset-1] != '\r' )
                {
                    offset += read(file_mkusb, &rx_buffer[offset], TAILLE_BUFER-offset);
                }
            			//printf("toto 5\n");
                // Decodes the packet
                Decode64(buf, rx_buffer, offset,3,offset);
                //sem_wait(&mutex_analog);
                memset(AnalogData,   0, ANALOG_SIZE  *sizeof(int)  );
                memset(&rx_buffer, 0, TAILLE_BUFER);
                // Converts data to integer
                for (i = 0; i < ANALOG_SIZE; i++)
                {
                    AnalogData[i] = Data2Int(buf, (i * 2) + 2);
                    
                }
                //sem_post(&mutex_analog);
                		//printf("toto \n");
                //printf("%d %d %d %d \n",AnalogData[0],AnalogData[1],AnalogData[30],AnalogData[31]);
                setHeader( &mc, 0, 0,SEND_INFO, 0 );
                
                InfoEncode(&mc, AnalogData, ANALOG_SIZE);
                
                //n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
            
                //MCDecode(&mc);
                
            }
		
		/*
		 * TODO traiter erreur de la requete si besoin
		 * pas de timeout reponse useless
		 **/
            info = 1;
		/*} else {
            
            printf("get info\n");
            get_info_GPGGA(buf);
            printf("decode info\n");
            gpgga = decode_GPGGA(buf);
            
            /*
            printf("%s\n", gg.gpgga_latitude);
            printf("%s\n", gg.gpgga_longitude);
            printf("%s\n", gg.gpgga_n_sat);
            printf("%s\n", gg.gpgga_accuracy_horizontal);
            printf("%s\n", gg.gpgga_altitude);
            */
            /*setHeader(&mc, 0, 0, SEND_GPS_INFO, 0);
            //MCEncode(&mc);
            GPSEncode(&mc, gpgga);
            
           /* printf("%s\n", mc.mc_data);
            printf("%s\n", &mc.mc_data[HEADER_SIZE]);*/
            
           /* printf("send gps info: %s\n", buf);
            info = 0;
        }*/

        sendData(mc, nt.nt_port, nt.nt_ip);
		
	}
	
	close(sock);
}
