#ifndef _H_PILOTAGE_
#define _H_PILOTAGE_

#include "fc_com.h"

struct str_ExternControl ExternControl;

void init_pilotage();
void set_Nick(signed char nick);
void set_Roll(signed char roll);
void set_Yaw(signed char yaw);
void set_Gas(unsigned char gas);
void envoi_pilotage(int file);
void set_active();
#endif
