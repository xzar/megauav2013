#include "tools.h"

/*
 * complement to 2 for int
 */
int complementInt(char b) {
	return b >= 0 ? b : b + 256;
}

/*
 * complement to 2 for long
 */
long complementLong(char b) {
	return b >= 0 ? b : b + 256;
}

/*
 * return the low char of int
 */
char intTo255(int i) {
	char b;
	
	b = (char) ( (i & 0x000000ff) );
	
	return b;
}

/*
 * fill buffer with the int chars
 * return -1 if size is greater than sizeof(int)
 */ 
int convertIntTochar(int i, char * buffer, int size)
{
	
	if ((unsigned int)size > sizeof(int))
	{
		return -1;
	}
	
	buffer[0] = (char) ( (i & 0x000000ff) );
	buffer[1] = (char) ( (i & 0x0000ff00) >> 8);
	buffer[2] = (char) ( (i & 0x00ff0000) >> 16);
	buffer[3] = (char) ( (i & 0xff000000) >> 24);
	
	return 1;
}

/*
 * convert a part of table in int
 * return the int represented by the buffer
 */
int convertcharToInt(char * buffer, int index, int size)
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
 * fill buffer with the long chars
 * return -1 if size is greater than sizeof(long)
 */ 
int convertLongTochar(long l, char *buffer, int size)
{
	if ((unsigned int)size > sizeof(long))
	{
		return -1;
	}
	
	buffer[0] = (char) ( (l >> 0) & 0xff );
	buffer[1] = (char) ( (l >> 8) & 0xff );
	buffer[2] = (char) ( (l >> 16) & 0xff );
	buffer[3] = (char) ( (l >> 24) & 0xff );
	buffer[4] = (char) ( (l >> 32) & 0xff );
	buffer[5] = (char) ( (l >> 40) & 0xff );
	buffer[6] = (char) ( (l >> 48) & 0xff );
	buffer[7] = (char) ( (l >> 56) & 0xff );
	
	/*
	buffer[7] = (char) ( (l >> 0) & 0xff );
	buffer[6] = (char) ( (l >> 8) & 0xff );
	buffer[5] = (char) ( (l >> 16) & 0xff );
	buffer[4] = (char) ( (l >> 24) & 0xff );
	buffer[3] = (char) ( (l >> 32) & 0xff );
	buffer[2] = (char) ( (l >> 40) & 0xff );
	buffer[1] = (char) ( (l >> 48) & 0xff );
	buffer[0] = (char) ( (l >> 56) & 0xff );
	*/
	
	/*
	buffer[0] = (char) ( (l & 0x000000ff) );
	buffer[1] = (char) ( (l & 0x0000ff00) >> 8);
	buffer[2] = (char) ( (l & 0x00ff0000) >> 16);
	buffer[3] = (char) ( (l & 0xff000000) >> 24);
	buffer[4] = (char) ( (l & 0x000000ff) >> 32);
	buffer[5] = (char) ( (l & 0x0000ff00) >> 40);
	buffer[6] = (char) ( (l & 0x00ff0000) >> 48);
	buffer[7] = (char) ( (l & 0xff000000) >> 56);
	*/
	
	return 1;
}

/*
 * convert a part of table in long
 * return the long represented by the buffer
 */
long convertcharToLong(char * buffer, int index, int size) {
	
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
int concatchars(char* tab1, int size1, char* tab2, int size2)
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
int cutchars (char* tab, int size, int index)
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
