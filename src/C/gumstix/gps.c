#include "gps.h"

int file_gps;

int open_gps()
{
	return serial_open(&file_gps, FILE_GPS, B9600);
}

void get_info_GPGGA(char * gpgga)
{
	int buf_size = 128;
	char trame[BUFFER_READ], *tmp_str, tmp_buf[buf_size];
	int ok = 1, offset=0;
	int match, err=0;
	regex_t preg_start;
	struct timeval tv1, tv2, res;
	const char *str_regex_start = "GPGGA";
	
	//printf("debug -1\n");
	
	err = regcomp (&preg_start, str_regex_start, REG_EXTENDED);
	if ( err != 0 ) {printf("err comp reg\n");return;}
	
	//printf("debug \n");
	memset(trame, 0 , BUFFER_READ);
	
	gettimeofday(&tv1, NULL);
	
	while(ok)
	{
		//memset(trame, 0 , BUFFER_READ);
		//printf("debug serial\n");
		//sleep(2);
		
		err=read(file_gps, &trame[offset], buf_size-offset);
		
		if (err == -1) { printf("error serial_read\n"); return;}
		offset+=err;
		
		//printf("trame : %s\n", trame);
		match = regexec (&preg_start, trame, 0, NULL, 0);
		
		if ( match == 0 ) 
		{
			tmp_str = strstr(trame, str_regex_start);
			//printf("MATCH 1 :: %s\n", tmp_str);
			ok=0;
		}
		
		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &res);
		//printf("%d,%d s\n", res.tv_sec, res.tv_usec);
		if (res.tv_sec >= 2){ printf("GPGGA not found timeout\n"); return;}
	}
	
	memset(tmp_buf, 0 , buf_size);
	memcpy( tmp_buf, tmp_str, strlen(tmp_str) );
	
	//printf("DEBUG 1\n");
	
	offset = strlen(tmp_buf) - 1;
	gettimeofday(&tv1, NULL);
	
	while( offset < buf_size )
	{
		//printf("DEBUG 2\n");
		err = read(file_gps, &tmp_buf[offset], buf_size-offset);
		
		if ( err == -1 )
		{
			printf("error gps 1\n");
			return;
		} else {
			offset+=err;
		}
		
		gettimeofday(&tv2, NULL);
		timersub(&tv2, &tv1, &res);
		//printf("%d,%d s\n", res.tv_sec, res.tv_usec);
		if (res.tv_sec >= 2){ printf("reading GPS info is too long\n"); return;}
	}
	
	strtok(tmp_buf, "$");
	
	memcpy(gpgga, tmp_buf, strlen(tmp_buf));
	
	regfree(&preg_start);
}

/*
 * convert a gpgga string in gpgga struct
 * 
 * Exemple of GPGGA info from serial port:
 * GPGGA,11373.00,4902.59765,N,00205.00247,E,1,09,1.07,73.1,M,46.0,M,,*65
 */
GPGGA decode_GPGGA(char * gpgga)
{
	GPGGA res;
	char buf[BUFFER_READ];
	int match, start, end, err;
	regex_t preg_start;
	size_t nmatch = 0, size;
	int i = 1;
	const char *str_regex = "([0-9]+\.[0-9]+,[A-Z]),([0-9]+\.[0-9]+,[A-Z]),[0-9],([0-9]{2}),([0-9]+\.[0-9]+),([0-9]+\.[0-9]+,[A-Z])(,)";
	regmatch_t *pmatch = NULL;
	
	err = regcomp (&preg_start, str_regex, REG_EXTENDED);
	if ( err != 0 ) {printf("err comp reg\n");return res;}
	
	nmatch = preg_start.re_nsub;
	//printf("%d\n", nmatch);
	pmatch = malloc (sizeof(*pmatch) * nmatch);
	
	match = regexec (&preg_start, gpgga, nmatch, pmatch, 0);
	
	if (match == 0)
	{
		for (; i < nmatch; i++)
		{
			start = pmatch[i].rm_so;
			end = pmatch[i].rm_eo;
			size = end - start;
			
			memset(buf, 0, BUFFER_READ);
			strncpy(buf, &gpgga[start], size);
			buf[size] = '\0';
			
			//printf("%s\n", buf);
			
			switch(i)
			{
				case 1://Latitude
					memcpy(res.gpgga_latitude, buf, BUFFER_READ);
					break;
				case 2://Longitude
					memcpy(res.gpgga_longitude, buf, BUFFER_READ);
					break;
				case 3://Number of Satellites
					memcpy(res.gpgga_n_sat, buf, BUFFER_READ);
					break;
				case 4://Accuracy of horizontal position 
					memcpy(res.gpgga_accuracy_horizontal, buf, BUFFER_READ);
					break;
				case 5://Altitude
					memcpy(res.gpgga_altitude, buf, BUFFER_READ);
					break;
			}
		}
	} else {
		printf("err GPGGA\n");
	}
	
	regfree(&preg_start);
	free(pmatch);
	return res;
}
