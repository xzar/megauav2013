
typedef char byte;

/*
 * complement to 2 for int
 */
int complementInt(byte b) ;

/*
 * complement to 2 for long
 */
long complementLong(byte b) ;

/*
 * return the low byte of int
 */
byte intTo255(int i) ;

/*
 * fill buffer with the int bytes
 * return -1 if size is greater than sizeof(int)
 */ 
int convertIntToByte(int i, byte * buffer, int size);

/*
 * convert a part of table in int
 * return the int represented by the buffer
 */
int convertByteToInt(byte * buffer, int index, int size);

/*
 * fill buffer with the long bytes
 * return -1 if size is greater than sizeof(long)
 */ 
int convertLongToByte(long l, byte *buffer, int size) ;

/*
 * convert a part of table in long
 * return the long represented by the buffer
 */
long convertByteToLong(byte* buffer, int index, int size) ;

/*
 * concatenate tab2 in tab1
 * tab2 is not free
 * new size of tab1 is size1+size2
 * don't forget to update it.
 * return 0 if error
 */
int concatBytes(byte* tab1, int size1, byte* tab2, int size2);

/*
 * Keep the right part of tab, started at index
 * the left part is free
 * new size of tab is size - index
 * don't forget to update it.
 * return -1 if error
 */
int cutBytes (byte* tab, int size, int index);
