static byte[] convertIntToByte(int i) 
{
	unsigned char bytes[4];

	bytes[0] = (i >> 24) & 0xFF;
	bytes[1] = (i >> 16) & 0xFF;
	bytes[2] = (i >> 8) & 0xFF;
	bytes[3] = i & 0xFF;
	
	return bytes;
}

static int convertByteToInt(byte [] buffer) 
{		
	int integer;
	
	integer = bytes[0] | ( (int)bytes[1] << 8 ) | ( (int)bytes[2] << 16 ) | ( (int)bytes[3] << 24 );

	return integer;
}
