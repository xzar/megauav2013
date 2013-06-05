#ifndef _H_GPS_
#define _H_GPS_

#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include "serial_util.h"

#define BUFFER_READ 1024
#define FILE_GPS "/dev/ttyS0"

typedef struct{

	char gpgga_latitude[BUFFER_READ];
	char gpgga_longitude[BUFFER_READ];
	char gpgga_n_sat[BUFFER_READ];
	char gpgga_accuracy_horizontal[BUFFER_READ];
	char gpgga_altitude[BUFFER_READ];


}GPGGA;

/*
 * Global file gps descriptor.
 */
int file_gps;

/*
 * Open file descriptor with GPS info
 * file descriptor is global.
 */
int open_gps(char *file);

/*
 * read the GPGGA info
 * copy the info in gpgga string
 */
void get_info_GPGGA(char * gpgga);

/*
 * convert a gpgga string in gpgga struct
 */
GPGGA decode_GPGGA(char * gpgga);

#endif
