#ifndef _H_SERIAL_UTIL_
#define _H_SERIAL_UTIL_

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "var.h"

extern int file_mkusb;

int serial_open(int* , char* , speed_t );

int serial_write(int , char* , int );

int serial_read(int , char* , int );

#endif
