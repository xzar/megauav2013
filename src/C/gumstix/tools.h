
typedef char byte;

int complementInt(byte b) ;
long complementLong(byte b) ;
byte intTo255(int i) ;
byte* convertIntToByte(int i) ;
int convertByteToInt(byte* buffer, int index, int size) ;
byte* convertLongToByte(long l) ;
long convertByteToLong(byte* buffer, int index, int size) ;
byte* concatBytes(byte* tab1, byte* tab2) ;
byte* cutBytes (byte* tab, int index) ;
