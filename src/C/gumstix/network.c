#include "network.h"

/*
 * GLOBAL
 */
NetFifo globalNetFifo;

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
	/*
	while (1)
	{
		n = recvfrom (sock, buf, BUFFER_SIZE, 0, (struct sockaddr *)&exp_addr, &exp_len);
		//TODO MUTEK
		
		addNetFifo(&globalNetFifo, buf); 
		
	} 
	*/
	close(sock);
}


/*
void open_reseau()
{
	sock = socket(PF_INET, SOCK_DGRAM, 0) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (5000) ;
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
}

void attente_reseau(char* buf, int lenght)
{
	int n, exp_len ;
	n = recvfrom (sock, buf, lenght, 0, (struct sockaddr *)&exp_addr, &exp_len);
	
	//printf("message reseau =  %s \n",  buf) ;
}

void close_reseau()
{
	close (sock) ; 
}

void envoi_message(char* adresse, char* buf, int lenght)
{
	recv_addr.sin_addr.s_addr = inet_addr (adresse) ;
	recv_addr.sin_port = htons (5000) ;
	sendto (sock, buf, 24, 0, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
}
	
*/
