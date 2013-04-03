#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "var.h"

int sock;
struct sockaddr_in recv_addr, exp_addr ; 


void open_reseau();

void attente_reseau(char*,int);
void close_reseau();
void envoi_message(char*, char*, int);
