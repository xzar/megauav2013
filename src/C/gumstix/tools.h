
typedef char byte;

int complementInt(byte b) ;
long complementLong(byte b) ;
byte intTo255(int i) ;
byte* convertIntToByte(int i) ;
int convertByteToInt(byte* buffer, int index, int size) ;
byte* convertLongToByte(long l) ;
long convertByteToLong(byte* buffer, int index, int size) ;

/*
 * concatenate tab2 in tab1
 * tab2 is not free
 * new size of tab1 is size1+size2
 * return 0 if error
 */
int concatBytes(byte* tab1, int size1, byte* tab2, int size2) ;

/*
 * Keep the right part of tab, started at index
 * the left part is free
 * new size of tab is size - index
 * return -1 if error
 */
int cutBytes (byte* tab, int *size, int index) ;
