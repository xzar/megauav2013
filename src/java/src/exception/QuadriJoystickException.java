package exception;

public class QuadriJoystickException extends Exception
{
	/**
	 * 
	 */
	private static final long serialVersionUID = -8660724596876445941L;
	
	/**
	 * 
	 */
	public static final String ERR_COM = "ERR_COM : Quadri Joystick not found";
	
	/**
	 * Constructor
	 * @param p_o_Message Exception message
	 */
	public QuadriJoystickException( String p_o_Message )
	{
		super(p_o_Message);
	}
}
