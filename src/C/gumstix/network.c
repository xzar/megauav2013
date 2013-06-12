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
int sendImage(int port, const char *ip, char * imgRGB, int size)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in dest;
	int socklen = sizeof(dest);
	struct hostent* serv = NULL;
	char * Image;
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
	int exp_len;
	char buf[BUFFER_SIZE];
	struct timeval timeout;
	MuavCom mc;
	
	timeout.tv_sec=0;
	timeout.tv_usec=TIMEOUT_MS;
	
	sock = socket(PF_INET, SOCK_DGRAM, 0) ;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (nt.nt_port) ;
	
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	
	printf("receive command port : %d\n", nt.nt_port);
	
	while (1)
	{
		memset(buf,0,BUFFER_SIZE);
		recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
		
		
		
#ifdef DEBUG1
printf("received: %s\n", buf);
#endif
		memcpy(mc.mc_data, buf, BUFFER_SIZE);
		
		MCDecode(&mc);
		
		switch(mc.mc_request)
		{
			case PILOTE_REQ_MANUAL:
			
				sem_wait(&mutex_fifo);
				clearNetFifo(&globalNetFifo);
				sem_post(&mutex_fifo);

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

				sem_wait(&mutex_status);
				status = MODE_OFF;
				sem_post(&mutex_status);
				break;
			default:
				sem_wait(&mutex_fifo);
				addNetFifo(&globalNetFifo, buf);
				sem_post(&mutex_fifo);

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
	int buf[BUFFER_SIZE];
	//struct sockaddr_in recv_addr, exp_addr ;

	int offset,cpt = 0 ;
	
	int parameters[1];
	int i=0;
    MuavCom mc;
	
	/*sock = socket(PF_INET, SOCK_DGRAM, 0) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (nt.nt_port) ;
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;*/
	
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
			//sem_wait(&mutex_analog);
			memset(AnalogData,   0, ANALOG_SIZE  *sizeof(int)  );
			memset(&rx_buffer, 0, TAILLE_BUFER);
			// Converts data to integer
			for (i = 0; i < ANALOG_SIZE; i++)
			{
				AnalogData[i] = Data2Int(buf, (i * 2) + 2);
				
			}
			//sem_post(&mutex_analog);
			
			//printf("%d %d %d %d \n",AnalogData[0],AnalogData[1],AnalogData[30],AnalogData[31]);
			setHeader( &mc, 0, 0,SEND_INFO, 0 );
			
			InfoEncode(&mc, AnalogData, ANALOG_SIZE);
			
			//recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
		
			//MCDecode(&mc);
			
		}
		
		/*
		 * TODO traiter erreur de la requete si besoin
		 * pas de timeout reponse useless
		 **/

        sendData(mc, nt.nt_port, nt.nt_ip);
		
	}
	
	//close(sock);
}

/*
 * send the gps drone info to the control tower.
 * 
 */
void *th_sendGPS(void *data)
{
	Network_info nt = *(Network_info*)data;
	char buf[BUFFER_SIZE];
	GPGGA gpgga;
	MuavCom mc;
	int error = 0;
	
	open_gps();
	
	while(1)
	{
		initMuavCom(&mc);
		//printf("get info\n");
		error = get_info_GPGGA(buf);
		
		if ( error == -1 )
		{
			setHeader(&mc, 0, 0, SEND_GPS_INFO, ERR_GPS);
			MCEncode(&mc);
			sendData(mc, nt.nt_port, nt.nt_ip);
			continue;
		}
		
		//printf("decode info\n");
		gpgga = decode_GPGGA(buf, &error);
		
		if ( error == -1 )
		{
			setHeader(&mc, 0, 0, SEND_GPS_INFO, ERR_GPS);
			MCEncode(&mc);
			sendData(mc, nt.nt_port, nt.nt_ip);
			continue;
		}
		
		/*
		printf("%s\n", gg.gpgga_latitude);
		printf("%s\n", gg.gpgga_longitude);
		printf("%s\n", gg.gpgga_n_sat);
		printf("%s\n", gg.gpgga_accuracy_horizontal);
		printf("%s\n", gg.gpgga_altitude);
		*/
		setHeader(&mc, 0, 0, SEND_GPS_INFO, 0);
		//MCEncode(&mc);
		GPSEncode(&mc, gpgga);
		
		//printf("%s\n", mc.mc_data);
		//printf("%s\n", &mc.mc_data[HEADER_SIZE]);
		
		//printf("send gps info: %s\n", buf);
		
		sendData(mc, nt.nt_port, nt.nt_ip);
	}
	
	//close(sock);
}

void * th_sendImage(void * data)
{
	Network nt = *(Network*)data;
	MuavCom mc, mc2;
	int height=120, width=192;
	int offset=0;
	int size, n;
	unsigned char * image;
	char part[1];
	CvSize taille;
	IplImage* frame;
	IplImage* frameGray;
	
	int sock;
	struct sockaddr_in recv_addr, exp_addr ;
	int exp_len;
	char buf[BUFFER_SIZE];
	struct timeval timeout;
	
	timeout.tv_sec=0;
	timeout.tv_usec=TIMEOUT_MS;
	
	sock = socket(PF_INET, SOCK_DGRAM, 0) ;
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (nt.nt_port) ;
	
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	
	open_capture(0,width,height);
	
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 16);
	//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 12);
	
	frame = cvQueryFrame( capture );
	if ( ! frame  )
	{
		fprintf( stderr, "ERROR: pas d'image\n" );
		return 0;
	}
	taille = cvGetSize(frame);
	frameGray = cvCreateImage(taille,8,1);
	image = frameGray->imageData;
	
	size = ( taille.width * taille.height ) + HEADER_SIZE;
	int size_part = BUFFER_SIZE - HEADER_SIZE - 1;
	
	//printf("taille img : %d %d\n", taille.width, taille.height);
	
	char encodedData[BUFFER_SIZE];
	
	while(1)
	{
		part[0]=(char)1;
		initMuavCom(&mc);
		setHeader(&mc, 0, 0, SEND_IMG, 0);
		MCEncode(&mc);
		memset(encodedData, 0, BUFFER_SIZE);
		offset=0;
		
		//printf("debug img1\n");
		
		frame = cvQueryFrame( capture );
		if ( ! frame  )
		{
			fprintf( stderr, "ERROR: pas d'image\n" );
			return 0;
		}
		RGBTOGRAY_1CANAL(frame, frameGray);
		
		//printf("debug img2\n");
		
		do
		{
			memset(buf, 0, BUFFER_SIZE);
			//printf("debug img3\n");
			imageEncode(&mc, image, size_part, encodedData, offset, part);
			
			//printf("debug img4\n");
			sendImage(nt.nt_port, nt.nt_ip, encodedData, BUFFER_SIZE);
			//printf("debug img5\n");
			
			n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, (socklen_t *)&exp_len);
			
			if (n != -1)
			{
				
				
				memcpy(mc2.mc_data, buf, BUFFER_SIZE);
				
				MCDecode(&mc2);
				//printf("", );
				if ( mc2.mc_request == R_SEND_IMG ) 
				{
					printf("part %d\n", (int)mc2.mc_data[HEADER_SIZE]);
					offset = size_part * ( (int)mc2.mc_data[HEADER_SIZE] - 1 );
					part[0] = mc2.mc_data[HEADER_SIZE];
				}
			}
			
		} while (offset < size);
		
		usleep(500000);
	}
}
