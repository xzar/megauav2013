import java.net.DatagramPacket;


public class test {

	/**
	 * @param args
	 */
	
	static int complementByteToInt(byte b) {
	    return b >= 0 ? b : b + 256;
	}
	
	static byte[] convertIntToByte(int i) {
		byte[] buffer = new byte[4];
		buffer[0] = (byte) ( (i & 0x000000ff) );
		buffer[1] = (byte) ( (i & 0x0000ff00) >> 8);
		buffer[2] = (byte) ( (i & 0x00ff0000) >> 16);
		buffer[3] = (byte) ( (i & 0xff000000) >> 24);
		
		return buffer;
	}
	
	static int convertByteToInt(byte [] buffer) {
		
		int integer;
		
		integer = complementByteToInt(buffer[3])*256*256*256
				+complementByteToInt(buffer[2])*256*256
				+complementByteToInt(buffer[1])*256
				+complementByteToInt(buffer[0]);
				
		
		return integer;
	}
	
	public static void main(String[] args) {
		byte b = (byte) 0x87;
		Byte bb = b;
		//System.out.println(bb.longValue());
		
		Integer t1 = 155455;
		Integer t2 = 1554556458;
		
		byte [] ttt = convertIntToByte(t2);
		
		int toto = convertByteToInt(ttt);
		
		System.out.println(toto);
		
//		byte b1 = t1.byteValue();
//		byte b2 = t2.byteValue();
//		System.out.println(b1);
//		System.out.println(b2);
		
		DatagramPacket dp;
		//byte[] ff = new byte[2];
//		ff[0] =b1;
//		ff[1] =b2;
		
		dp = new DatagramPacket(ttt, ttt.length);
		
		byte[] res = dp.getData();
		
		System.out.println(convertByteToInt(ttt));
		
	    /*bufferToSend[1]=(byte) 35160 & 0x00FF; 
	    bufferToSend[2]=(byte) ( (35160 & 0xFF00) >> 8 ); 
	 
	    int monEntier=convertByteToInt(bufferToSend[2])*256+
	                  convertByteToInt(bufferToSend[1]); // attention les indices 1 et 2 étaient inversés !
	    System.out.println("monEntier  = " + monEntier); */
	
	 
	  
		
		

	}

}
