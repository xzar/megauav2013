#include "pilotage.h"

/*********************
*
*Fonction de modification 
* du externControl
*
*********************/
void init_pilotage()
{
#ifdef DEBUG2
printf("entrer init_pilotage pilotage.c\n");	
#endif

	ExternControl.Digital[0] = 0;
	ExternControl.Digital[1] = 0;
	ExternControl.RemoteButtons = 0;

	ExternControl.Nick = 0;
	ExternControl.Roll = 0;
	ExternControl.Gier = 0;
	ExternControl.Gas = 0;

	ExternControl.Hight = 0;
	ExternControl.free = 0;
	ExternControl.Frame = 0;
	ExternControl.Config = 1;


}


void set_Nick(signed char nick)
{
#ifdef DEBUG2
printf("entrer set_Nick pilotage.c\n");	
#endif

	ExternControl.Nick = nick;
	

}

void set_Roll(signed char roll)
{
#ifdef DEBUG2
printf("entrer set_Roll pilotage.c\n");	
#endif

		ExternControl.Roll = roll;

}


void set_Yaw(signed char yaw)
{
#ifdef DEBUG2
printf("entrer set_Yaw pilotage.c\n");	
#endif

		ExternControl.Gier = yaw;

}

void set_Gas(unsigned char gas)
{
#ifdef DEBUG2
printf("entrer set_gas pilotage.c\n");	
#endif

		ExternControl.Gas = gas;
}

void envoi_pilotage(int file)
{

	SendOutData('b', 0, (unsigned char *) &ExternControl, sizeof(struct str_ExternControl),file);

}

void reception_pilotage()
{

}


void set_active(int config)
{

	if(ExternControl.Config == 1 && config == -1) ExternControl.Config = 0;
	else if (ExternControl.Config == 0 && config == -1) ExternControl.Config = 1;
	else if(config == 0 || config == 1) ExternControl.Config = config;
	else printf("Erreur pilotage.c set_Active Config\n");

	



}
