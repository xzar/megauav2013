#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdio.h>

int main () 
{ 
	
	int sock, n, exp_len ;
	struct sockaddr_in recv_addr, exp_addr ; 
	char buf [80] ; 
	sock = socket(PF_INET, SOCK_DGRAM, 0) ; 
	bzero ((char *) &recv_addr, sizeof recv_addr) ;
	recv_addr.sin_family = AF_INET ;
	recv_addr.sin_addr.s_addr = INADDR_ANY ;
	recv_addr.sin_port = htons (5000) ;
	bind (sock, (struct sockaddr *)&recv_addr, sizeof recv_addr) ;
	n =recvfrom (sock, buf, 80, 0, (struct sockaddr *)&exp_addr, &exp_len) ;
	printf ("%s a envoye le message %s /n", inet_ntoa (exp_addr), buf) ;
	close (sock) ; 
	return 0 ; 
	
}
