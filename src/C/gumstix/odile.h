#ifndef _H_ODILE_
#define _H_ODILE_

#include "fc_com.h"
#include "pilotage.h"
#include "network.h"
#include <sys/time.h>
#include "image.h"

void take_off(int altitude );

int deplacement_zero();

void pilote_IA(vecteur vecteurMoy,int timeOut,int Gas);
int converge();
int prise_photo();

#endif
