#include "tools.h"
#include <math.h>

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

byte* concatBytes(byte* tab1, byte* tab2) {
	int length_tab1 = sizeof(tab1)/sizeof(byte);
	int length_tab2 = sizeof(tab2)/sizeof(byte);
	
	byte* ret = (byte)malloc(length_tab1+length_tab2);
	
	int i = 0;
	
	while (i < length_tab1) {
		ret[i] =  tab1[i]; 
		i++;
	}
	
	int j = 0;
	while (i < length_tab1+length_tab2) {
		ret[i] =  tab1[j];
		i++;
		j++;
	}
	
	return ret;
}

byte* cutBytes (byte* tab, int index) {
	
	int i;
	int length = sizeof(tab)/sizeof(byte);
	int remain_length = length-index;
	
	if (remain_length == 0) {
		return -1;
	}
	
	byte* remain = (byte)malloc(length-index);
	
	for (i = 0; i < remain_length; i++) {
		remain[i] = tab[index+i];
	}
	return remain;
}
