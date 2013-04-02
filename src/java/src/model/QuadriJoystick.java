package model;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

import exception.QuadriJoystickException;
import net.java.games.input.Component;
import net.java.games.input.Controller;
import net.java.games.input.ControllerEnvironment;
import net.java.games.input.Component.Identifier;

public class QuadriJoystick
{
	/**
	 * 
	 */
	public static final String JOYSTICK_NAME = "THRUSTMASTER Top Gun Afterburner";
	
	/**
	 * 
	 */
	private Controller m_o_Controller;

	public QuadriJoystick() throws QuadriJoystickException
	{
		int 			l_i_Iter;
        Controller[] 	l_o_Controllers;
        
        // Gets available controllers
        l_o_Controllers = ControllerEnvironment.getDefaultEnvironment().getControllers();
        
        // For each controllers
        for(l_i_Iter = 0; l_i_Iter < l_o_Controllers.length && m_o_Controller == null; l_i_Iter++)
        {
        	// Checks the controller name
            if( l_o_Controllers[l_i_Iter].getName().equalsIgnoreCase( JOYSTICK_NAME ) )
            {
            	m_o_Controller = l_o_Controllers[l_i_Iter];
            }
        }
        
        // Throws exception if controller not found
        if( !isConnected() )
        {
        	throw new QuadriJoystickException( QuadriJoystickException.ERR_COM );
        }
	}
	
    public boolean isConnected()
    {
        try
        {
            return m_o_Controller.poll();
        }
        catch (Exception e)
        {
            return false;
        }
    }
    
	/**
	 * 
	 * @return
	 */
	public float getGas()
	{
		if( isConnected() )
		{
			return getIdentifierValue( Component.Identifier.Axis.SLIDER );
		}
		else
		{
			return 0;
		}
	}
	
	/**
	 * 
	 * @return
	 */
	public float getRoll()
	{
		if( isConnected() )
		{
			return getIdentifierValue( Component.Identifier.Axis.X );
		}
		else
		{
			return 0;
		}
	}
	
	/**
	 * 
	 * @return
	 */
	public float getNick()
	{
		if( isConnected() )
		{
			return getIdentifierValue( Component.Identifier.Axis.Y );
		}
		else
		{
			return 0;
		}
	}
	
	/**
	 * 
	 * @return
	 */
	public float getYaw()
	{
		if( isConnected() )
		{
			return getIdentifierValue( Component.Identifier.Axis.RZ );
		}
		else
		{
			return 0;
		}
	}
	
	/**
	 * 
	 * @return
	 */
	public float getZ()
	{
		if( isConnected() )
		{
			return getIdentifierValue( Component.Identifier.Axis.Z );
		}
		else
		{
			return 0;
		}
	}
	
	/*
	 * 
	 */
    public float getHatSwitchPosition()
    {
		if( isConnected() )
		{
			return getIdentifierValue( Component.Identifier.Axis.POV );
		}
		else
		{
			return 0;
		}
    }
	
	/**
	 * 
	 * @param l_o_Identifier
	 * @return
	 */
	public float getIdentifierValue(Identifier l_o_Identifier)
	{
		float res = m_o_Controller.getComponent( l_o_Identifier ).getPollData();
		return res;
	}
	
	public static void main(String[] args) throws IOException
	{
		
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
		byte[] tab = new byte[4];
		try
		{
			int i =20;
			QuadriJoystick l_o_Joystick = new QuadriJoystick();
			
			while(true)
			{
				
				int [] tib = new int [4];
				System.out.println("Nick = " + l_o_Joystick.getNick());
				System.out.println("Roll = " + l_o_Joystick.getRoll());
				System.out.println("Yaw = " + l_o_Joystick.getYaw());
				System.out.println("Gas = " + l_o_Joystick.getGas());
				System.out.println("\n");
				//sendData = testAlex.convertIntToByte(intToSend);
				
				tib[0] = (int)(l_o_Joystick.getNick()*127);
				tib[1] = (int)(l_o_Joystick.getRoll()*127);
				tib[2] = (int)(l_o_Joystick.getYaw()*127);
				tib[3] = (int)(l_o_Joystick.getGas()*127);
				System.out.println("nick =" + Integer.toString(tib[0])+"roll =" + Integer.toString(tib[1])+"yaw =" + tib[2]+"gas =" + tib[3]);
				
				tab[0] = Byte.decode(Integer.toString((tib[0])));
				tab[1] = Byte.decode(Integer.toString(tib[1]));
				tab[2] = Byte.decode(Integer.toString(tib[2]));
				tab[3] = Byte.decode(Integer.toString(tib[3]));
				System.out.println("nick =" + tab[0]+"roll =" + tab[1]+"yaw =" + tab[2]+"gas =" + tab[3]);
				DatagramPacket sendPacket = new DatagramPacket(tab, tab.length, IPAddress, port);
				
				clientSocket.send(sendPacket);
				/*DatagramPacket receivePacket = new DatagramPacket(receiveData, receiveData.length);
				
				clientSocket.receive(receivePacket);
				
				String modifiedSentence = new String(receivePacket.getData());
				
				System.out.println("FROM SERVER:" + modifiedSentence);*/
				
				Thread.sleep(100);
				i--;
			}
			//clientSocket.close();
		}
		catch (QuadriJoystickException l_o_Exception)
		{
			
			System.out.println(l_o_Exception.getMessage());
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
}
