#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "reseau.h"
int serial_open(int* , char* );

int serial_write(int , char* , int );

int serial_read(int , char* , int );
