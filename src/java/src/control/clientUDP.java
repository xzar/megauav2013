package control;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class clientUDP {

	/**
	 * @param args
	 * @throws Exception
	 */
	public static void main(String[] args) throws Exception {
		
		int port, intToSend;
		InetAddress IPAddress;
		byte[] sendData = new byte[4];
		byte[] receiveData = new byte[4];
		
		BufferedReader inFromUser = new BufferedReader(new InputStreamReader(System.in));
		DatagramSocket clientSocket = new DatagramSocket();
		
		
	/*	Byte b = new Byte("127");
		
		System.out.println(b);
		
		System.exit(0);*/
		
		
		if ( args.length >= 2 ) {
			IPAddress = InetAddress.getByName(args[0]);
			port = Integer.parseInt(args[1]);
		} else {
			System.out.println("IP:");
			IPAddress = InetAddress.getByName(inFromUser.readLine());
			System.out.println("PORT:");
			port = Integer.parseInt(inFromUser.readLine());
		}

		//System.out.println("INT TO SEND");
		//intToSend = Integer.parseInt(inFromUser.readLine());
		
		byte toto = Byte.decode("3");
		
		//byte toto = testAlex.intTo255(intToSend);
		
		//System.out.println("toto:"+toto);
		//System.exit(0);
		
		byte[] tab = new byte[4];
		
		tab[0] = toto;
		tab[1] = toto;
		tab[2] = toto;
		tab[3] = toto;			
		
		
		//sendData = testAlex.convertIntToByte(intToSend);
		
		DatagramPacket sendPacket = new DatagramPacket(tab, tab.length, IPAddress, port);
		
		clientSocket.send(sendPacket);
		/*DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
		
		clientSocket.receive(receivePacket);
		
		String modifiedSentence = new String(receivePacket.getData());
		
		System.out.println("FROM SERVER:" + modifiedSentence);*/
		clientSocket.close();

	}

}
