#include "tools.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int complementInt(byte b) {
	return b >= 0 ? b : b + 256;
}

long complementLong(byte b) {
	return b >= 0 ? b : b + 256;
}

byte intTo255(int i) {
	byte b;
	
	b = (byte) ( (i & 0x000000ff) );
	
	return b;
}

byte* convertIntToByte(int i) {
	byte* buffer = (byte)malloc(4);
	buffer[0] = (byte) ( (i & 0x000000ff) );
	buffer[1] = (byte) ( (i & 0x0000ff00) >> 8);
	buffer[2] = (byte) ( (i & 0x00ff0000) >> 16);
	buffer[3] = (byte) ( (i & 0xff000000) >> 24);
	
	return buffer;
}

int convertByteToInt(byte * buffer, int index, int size) {
	
	int integer = 0;
	int j=0;
	int i = 0;
	for (i = index; i < index+size; i++) {
		integer += complementInt(buffer[i])*(int)pow(256, j);
		j++;
	}
	
	return integer;
}

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
 * return 0 if error
 */
int concatBytes(byte* tab1, int size1, byte* tab2, int size2)
{
    tab1 = realloc(tab1, size1 + size2);

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
 * return -1 if error
 *
 * a lire et corriger le realloc
 * http://ilay.org/yann/articles/mem/mem1.html
 */
int cutBytes (byte* tab, int* size, int index)
{
	
	int i;
	int remain_length = (*size)-index;
	
	if (remain_length == 0) {
		return -1;
	}
	
	for (i = 0; i < remain_length; i++) {
		tab[i] = tab[index+i];
	}

    for (i = 0; i < remain_length; i++) {
		printf("remain:%c\n", tab[i]);
	}

    tab = realloc(tab, remain_length);
    
    printf("addr in fct:%d\n", tab);
    
    *size = remain_length;
    
	return 1;
}
