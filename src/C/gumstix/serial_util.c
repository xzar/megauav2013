#include "serial_util.h"
int file_mkusb;
//#define DEBUG2
/*
 * mkusb : B57600
 * gps : B9600
 */


int serial_open(int* file, char* nameFile, speed_t baudRate)
{
#ifdef DEBUG2
printf("entrer open serial_util.c\n");	
#endif
	
	struct termios tops;
	

	//speed_t baudRate;
	
	
	tops.c_cc[0] = VERASE;
	//baudRate = B57600;

	*file = open (nameFile, O_RDWR | O_NOCTTY | O_SYNC);
	if (*file < 0)
	{
		
		printf ("error read, %d\n", *file);
		return -1;
	}
	tcgetattr (*file, &tops);
	cfmakeraw(&tops); 
	tops.c_cflag &= ~(IXON|IXOFF);
    tops.c_cflag &= ~(CSIZE | PARENB);
    tops.c_cflag |= CS8; 
	cfsetispeed (&tops, baudRate);
	cfsetospeed (&tops, baudRate);
	tcsetattr (*file, TCSANOW, &tops); 	
	
	return 1;
	
	


}

int serial_write(int file, char* buf, int length)
{
#ifdef DEBUG2
printf("entrer write serial_util.c\n");	
#endif
	int retour;
	if((retour = write(file,buf,length)) < 0)
	{
	
		printf("Erreur Write, %d\n",retour);
	
		return retour;
	}
	return retour;

}

int serial_read(int file, char* buf, int lenght)
{
#ifdef DEBUG2
printf("entrer read serial_util.c\n");	
#endif
	int n;
	if((n = read (file, buf, lenght)) < 0)
	{
	#ifdef DEBUG1
	printf("Erreur Read, %d \n",n);
	#endif
		//sleep(100);
		return -1;
	}
    #ifdef DEBUG1
	printf("lu lenght = %d \n",n);
	#endif
	return n;
}

void serial_close(int file){
	close(file);
}
