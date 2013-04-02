package testPackage;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

public class serverUDP {
	public static void main(String[] args) throws IOException {
		DatagramSocket serverSocket = new DatagramSocket(1337);
		byte[] receiveData = new byte[4];
		byte[] sendData = new byte[1024];
		while (true) {
			
			DatagramPacket receivePacket = new DatagramPacket(receiveData,
					receiveData.length);
			
			serverSocket.receive(receivePacket);
			int rec = testAlex.convertByteToInt(receivePacket.getData());
			
			System.out.println("RECEIVED: " + rec);
			
			
			
			InetAddress IPAddress = receivePacket.getAddress();
			int port = receivePacket.getPort();
			
			
			sendData = receiveData;
			
			DatagramPacket sendPacket = new DatagramPacket(sendData,
					sendData.length, IPAddress, port);
			
			serverSocket.send(sendPacket);
		}
	}
}
