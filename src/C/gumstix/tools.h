#ifndef _H_TOOLS_
#define _H_TOOLS_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * complement to 2 for int
 */
int complementInt(char b) ;

/*
 * complement to 2 for long
 */
long complementLong(char b) ;

/*
 * return the low char of int
 */
char intTo255(int i) ;

/*
 * fill buffer with the int chars
 * return -1 if size is greater than sizeof(int)
 */ 
int convertIntTochar(int i, char * buffer, int size);

/*
 * convert a part of table in int
 * return the int represented by the buffer
 */
int convertcharToInt(char * buffer, int index, int size);

/*
 * fill buffer with the long chars
 * return -1 if size is greater than sizeof(long)
 */ 
int convertLongTochar(long l, char *buffer, int size) ;

/*
 * convert a part of table in long
 * return the long represented by the buffer
 */
long convertcharToLong(char* buffer, int index, int size) ;

/*
 * concatenate tab2 in tab1
 * tab2 is not free
 * new size of tab1 is size1+size2
 * don't forget to update it.
 * index : where to start concat.
 * return 0 if error
 */
int concatchars(char* tab1, int size1, char* tab2, int size2);

/*
 * Keep the right part of tab, started at index
 * the left part is free
 * new size of tab is size - index
 * don't forget to update it.
 * return -1 if error
 */
int cutchars (char* tab, int size, int index);
#endif
