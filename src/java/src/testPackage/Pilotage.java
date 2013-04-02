package testPackage;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class Pilotage {
	private DatagramPacket dp;
	private DatagramSocket ds;
	private InetAddress addr;
	private int port;
	private int maxBytes = 4;
	
	public Pilotage(String addr, int port, int maxBytes) throws UnknownHostException, SocketException {
		this.addr = InetAddress.getByName(addr);
		this.port = port;
		this.maxBytes = maxBytes;
		ds = new DatagramSocket();
	}
	
	public Pilotage(String addr, int port) throws UnknownHostException, SocketException {
		this.addr = InetAddress.getByName(addr);
		this.port = port;
		ds = new DatagramSocket();
	}
	
	public InetAddress getAddr() {
		return addr;
	}
	public void setAddr(InetAddress addr) {
		this.addr = addr;
	}
	public int getPort() {
		return port;
	}
	public void setPort(int port) {
		this.port = port;
	}
	
	public void sendOrder (byte[] orders) throws Exception {
		
		if (orders.length != maxBytes) throw new Exception("Fail !!!!!!!");
		
		dp = new DatagramPacket(orders, maxBytes, addr, port);
		
		ds.send(dp);
		
	}
	
	public void stop() {
		ds.close();
	}
	
	public static void main(String[] args) {
		
	}
}
