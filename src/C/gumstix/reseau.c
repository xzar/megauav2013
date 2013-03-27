#include "reseau.h"


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
	
	printf("message reseau =  %s \n",  buf) ;
}

void close_reseau()
{

	close (sock) ; 

}	
	
