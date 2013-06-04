#include "odile.h"

void take_off(int altitude )
{
	int nick, roll, accNick, accRoll, alti,vario;
	signed char cmdNick = 0, cmdRoll = 0, cmdYaw = 0;
	unsigned char cmdGas = 255;
	unsigned char GasInter = 0;
	while (1)
	{
		sem_wait(&mutex_analog);
		nick = AnalogData[0];
		roll = AnalogData[1];
		accNick = AnalogData[2] - AnalogData[0];
		accRoll = AnalogData[3] - AnalogData[1];
		alti = AnalogData[5];
		vario = AnalogData[6];
		sem_post(&mutex_analog);
//		cmdGas
		//nick+=20;
		if(nick > 5 || nick < -5 ){
			if(nick < 0)
				cmdNick = 15;
			else cmdNick = -15;
		}else cmdNick = 0;

		if(roll > 5 || roll < -5 ){
			if(roll < 0)
				cmdRoll = 15;
			else cmdRoll = -15;
		}else cmdRoll = 0;
	
		printf("**********[COMMANDE]******* : \ncmdNick = %d\ncmdRoll = %d\ncmdYaw = %c \ncmdGas = %c\n",cmdNick,cmdRoll,cmdYaw,cmdGas);
		printf("**********[DEBUG]********** : \nnick = %d\nroll = %d\naccNick = %d\naccRoll = %d\nalti = %d\nvario = %d\n\n\n",nick,roll,accNick,accRoll,alti,vario);
		usleep(100000);
		set_Nick( (signed char) cmdNick );
		set_Roll( (signed char) cmdRoll );
		set_Yaw( (signed char) cmdYaw ); 
		set_Gas( (unsigned char) cmdGas );
		envoi_pilotage(file_mkusb);
	}

}
