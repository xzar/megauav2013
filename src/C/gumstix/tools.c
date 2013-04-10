#include "tools.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


/*
 * complement to 2 for int
 */
int complementInt(byte b) {
	return b >= 0 ? b : b + 256;
}

/*
 * complement to 2 for long
 */
long complementLong(byte b) {
	return b >= 0 ? b : b + 256;
}

/*
 * return the low byte of int
 */
byte intTo255(int i) {
	byte b;
	
	b = (byte) ( (i & 0x000000ff) );
	
	return b;
}

/*
 * fill buffer with the int bytes
 * return -1 if size is greater than sizeof(int)
 */ 
int convertIntToByte(int i, byte * buffer, int size)
{
	
	if (size > sizeof(int))
	{
		return -1;
	}
	
	buffer[0] = (byte) ( (i & 0x000000ff) );
	buffer[1] = (byte) ( (i & 0x0000ff00) >> 8);
	buffer[2] = (byte) ( (i & 0x00ff0000) >> 16);
	buffer[3] = (byte) ( (i & 0xff000000) >> 24);
	
	return 1;
}

/*
 * convert a part of table in int
 * return the int represented by the buffer
 */
int convertByteToInt(byte * buffer, int index, int size)
{
	
	int integer = 0;
	int j=0;
	int i = 0;
	for (i = index; i < index+size; i++) {
		integer += complementInt(buffer[i])*(int)pow(256, j);
		j++;
	}
	
	return integer;
}

/*
 * fill buffer with the long bytes
 * return -1 if size is greater than sizeof(long)
 */ 
int convertLongToByte(long l, byte *buffer, int size)
{
	if (size > sizeof(long))
	{
		return -1;
	}
	
	buffer[0] = (byte) ( (l >> 0) & 0xff );
	buffer[1] = (byte) ( (l >> 8) & 0xff );
	buffer[2] = (byte) ( (l >> 16) & 0xff );
	buffer[3] = (byte) ( (l >> 24) & 0xff );
	buffer[4] = (byte) ( (l >> 32) & 0xff );
	buffer[5] = (byte) ( (l >> 40) & 0xff );
	buffer[6] = (byte) ( (l >> 48) & 0xff );
	buffer[7] = (byte) ( (l >> 56) & 0xff );
	
	return 1;
}

/*
 * convert a part of table in long
 * return the long represented by the buffer
 */
long convertByteToLong(byte * buffer, int index, int size) {
	
	long l = 0;
	int j=0;
	int i = 0;
	for (i = index; i < index+size; i++) {
		l += complementLong(buffer[i])*(long)pow(256, j);
		j++;
	}
	return l;
}

/*
 * concatenate tab2 in tab1
 * tab2 is not free
 * new size of tab1 is size1+size2
 * don't forget to update it.
 * return 0 if error
 */
int concatBytes(byte* tab1, int size1, byte* tab2, int size2)
{
    int i = size1;
	int j = 0;
    
	while (i < size1 + size2)
    {
		tab1[i] =  tab2[j];
		i++;
		j++;
	}
    
	return 1;
}

/*
 * Keep the right part of tab, started at index
 * the left part is free
 * new size of tab is size - index
 * don't forget to update it.
 * return -1 if error
 */
int cutBytes (byte* tab, int size, int index)
{
	int i;
	int remain_length = size-index;
	
	if (remain_length == 0) {
		return -1;
	}
	
	for (i = 0; i < remain_length; i++) {
		tab[i] = tab[index+i];
	}
	return 1;
}
