#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <string.h> 
#include <strings.h> 

int main () 
{
	int sock ;
	struct sockaddr_in recv_addr, exp_addr ;
	char buf [80] ; 
	sock = socket (PF_INET, SOCK_DGRAM, 0) ;
	bzero ((char *) &exp_addr, sizeof exp_addr) ;
	exp_addr.sin_family = AF_INET ;
	exp_addr.sin_addr.s_addr = INADDR_ANY ;
	bind (sock, (struct sockaddr *)&exp_addr, sizeof exp_addr) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = inet_addr ("172.22.136.18") ;
	recv_addr.sin_port = htons (5000) ;
	strncpy (buf, "Coucou, c'est l'envoyeur", 24) ;
	sendto (sock, buf, 24, 0, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	close (sock); 
	return 0 ; 
}
