#include "odile.h"

void take_off(int altitude )
{
	int nick, roll, accNick, accRoll, alti,vario;
	signed char cmdNick = 0, cmdRoll = 0, cmdYaw = 0;
	int initNick, initRoll;
	unsigned char cmdGas = 255;
	//unsigned char GasInter = 0;
	
	initNick = AnalogData[0];
	initRoll = AnalogData[1];
	while (status == MODE_AUTO ){

		//sem_wait(&mutex_analog);
		nick = AnalogData[0] ;
		roll = AnalogData[1] ;
		accNick = AnalogData[2] - AnalogData[0];
		accRoll = AnalogData[3] - AnalogData[1];
		alti = AnalogData[5];
		vario = AnalogData[6];
		//sem_post(&mutex_analog);

	//		cmdGas
		//nick+=20;
		/*if(nick > 5 || nick < -5 ){
			if(nick < 0)
				cmdNick = 50;
			else cmdNick = -50;
		}else cmdNick = 0;

		if(roll > 5 || roll < -5 ){
			if(roll < 0)
				cmdRoll = 50;
			else cmdRoll = -50;
		}else cmdRoll = 0;*/
		cmdNick = (signed char) -nick/5;
		cmdRoll = (signed char) -roll/5;

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





void pilote_IA(vecteur vecteurMoy,int timeOut,int seuil,int norme,int Gas){

		char nick, roll, yaw=0;
		unsigned char gas =  Gas;

		if(vecteurMoy.y > seuil){
			nick = (char)-norme;
		}
		if(vecteurMoy.y < -seuil){
			nick = (char)norme;
		}
		if(vecteurMoy.x > seuil ){

			roll = (char)-norme;
		}
		
		if(vecteurMoy.x < -seuil){
			roll = (char)norme;
		}
#ifdef DEBUGIA1	
printf("commande moteur : Nick = %d, Roll = %d, Yaw = %d, Gas = %d",nick,roll,yaw,gas);
#endif	
		set_Nick( (signed char) nick );
		set_Roll( (signed char) roll );
		set_Yaw( (signed char) yaw ); 
		set_Gas( (unsigned char) gas );
		envoi_pilotage(file_mkusb);

		nick =0;
		roll =0;
		usleep(timeOut);

		set_Nick( (signed char) nick );
		set_Roll( (signed char) roll );
		set_Yaw( (signed char) yaw ); 
		set_Gas( (unsigned char) gas );
		envoi_pilotage(file_mkusb);

}


int deplacement_zero()
{
#ifdef DEBUG2
printf("entrer deplacement_zero odile.c\n");	
#endif

#ifdef DEBUGIA1
struct timeval start,stop,res;
#endif
	
	open_capture(0,160,120);
	if ( !capture ) 
	{
		fprintf( stderr, "ERROR: capture is NULL \n" );
		getchar();
		return -1;
	}
	//recuperation d une premiere image pour avoir les dimensions
	IplImage* frame = cvQueryFrame( capture );
	//recuperation des dimensions de l image
	CvSize taille = cvGetSize(frame);
	int tailleInt = taille.width * taille.height;

	//creation des image et matrices intermediaires
	IplImage* frameGray = cvCreateImage(taille,8,1);
	float* matriceGradX = malloc(sizeof(float)*tailleInt);
	float* matriceGradY = malloc(sizeof(float)*tailleInt);
	float* matriceHarris= malloc(sizeof(float)*tailleInt);
	float** tabHarris = (float **)malloc(sizeof(float*)*_NBHARRIS);
	float** tabHarrisPrec = (float **)malloc(sizeof(float*)*_NBHARRIS);
	char * imageData;
	int j;

	for(j=0;j<_NBHARRIS;j++)
    {
	   tabHarris[j]=(float*)malloc(sizeof(float)*3);
		
    }
	for(j=0;j<_NBHARRIS;j++)
    {
	   tabHarrisPrec[j]=(float*)malloc(sizeof(float)*3);
		
    }
	vecteur *tabVect = (vecteur*) malloc(sizeof(vecteur)*_NBHARRIS);
	vecteur vecteurMoy;
	vecteurMoy.x = 0; vecteurMoy.y=0;
	int nbVect = 0;
	
	
	
	while(status == MODE_AUTO)
	{
#ifdef DEBUGIA1	
gettimeofday(&start,NULL);
#endif
		//Recuperation de l image de la camera
		frame = cvQueryFrame( capture );
	
		if ( ! frame  )
		{
			fprintf( stderr, "ERROR: pas d'image\n" );
			return -1;
		}
		
		imageData = frameGray->imageData;

		
		//Passage en niveau de gris	
		RGBTOGRAY_1CANAL(frame, frameGray);

		
		//Sobel pour les gradients
		SobelHV(imageData,taille.height, taille.width, matriceGradX, matriceGradY);
		
		
		//Detecteur de Harris
		harris(matriceHarris, matriceGradX, matriceGradY, tailleInt);
	
		//Maxima de Harris
		getMaxima(matriceHarris, taille.height, taille.width, tabHarris, _NBHARRIS);
		
		
		//Matching des points pour trouver les vecteurs
		nbVect = calcul_vecteur_interet(tabHarrisPrec, tabHarris, _NBHARRIS, _SEUIL_DISTANCE, tabVect);
		
		
		//Moyenne des vecteurs
		calcul_moyenne_vecteur(tabVect, nbVect,&vecteurMoy);
		
		for(j=0;j<_NBHARRIS;j++)
  		{

			  tabHarrisPrec[j][0] = tabHarris[j][0];
			  tabHarrisPrec[j][1] = tabHarris[j][1];
			  tabHarrisPrec[j][2] = tabHarris[j][2];
		}
		
#ifdef DEBUGIA1		
		printf("nbVecteur Match = %d \nVecteur moyen  = %d %d\n",nbVect,vecteurMoy.x,vecteurMoy.y);
		gettimeofday(&stop,NULL);
		timersub(&stop, &start, &res);
		printf("time Harris= %d %d\n",(int)res.tv_sec, (int)res.tv_usec);
#endif

		pilote_IA(vecteurMoy,1000,2,10, 255);
				
	}

	cvReleaseCapture( &capture );

#ifdef DEBUG2
printf("FIN deplacement_zero odile.c\n");	
#endif
	return 0;


}

int converge()
{
//calcul_vecteur_barycentre ( tabHarris, _NBHARRIS, &vecteurMoy, taille.height,taille.width);
#ifdef DEBUG2
printf("entrer converge odile.c\n");	
#endif

#ifdef DEBUGIA1
struct timeval start,stop,res;
#endif

	open_capture(0,160,120);
	if ( !capture ) 
	{
		fprintf( stderr, "ERROR: capture is NULL \n" );
		getchar();
		return -1;
	}
	
	//recuperation d une premiere image pour avoir les dimensions
	IplImage* frame = cvQueryFrame( capture );
	//recuperation des dimensions de l image
	CvSize taille = cvGetSize(frame);
	int tailleInt = taille.width * taille.height;

	//creation des image et matrices intermediaires
	IplImage* frameGray = cvCreateImage(taille,8,1);

	float* matriceGradX = malloc(sizeof(float)*tailleInt);
	float* matriceGradY = malloc(sizeof(float)*tailleInt);
	float* matriceHarris= malloc(sizeof(float)*tailleInt);
	float** tabHarris = (float **)malloc(sizeof(float*)*_NBHARRIS);
	float** tabHarrisPrec = (float **)malloc(sizeof(float*)*_NBHARRIS);
	char * imageData;
	int j;

	for(j=0;j<_NBHARRIS;j++)
    {
	   tabHarris[j]=(float*)malloc(sizeof(float)*3);
		
    }
	for(j=0;j<_NBHARRIS;j++)
    {
	   tabHarrisPrec[j]=(float*)malloc(sizeof(float)*3);
		
    }
	vecteur vecteurMoy;
	vecteurMoy.x = 0; vecteurMoy.y=0;
	int nbVect = 0;
		
	while(status == MODE_AUTO)
	{
#ifdef DEBUGIA1	
gettimeofday(&start,NULL);
#endif
		//Recuperation de l image de la camera
		frame = cvQueryFrame( capture );
	
		if ( ! frame  )
		{
			fprintf( stderr, "ERROR: pas d'image\n" );
			return -1;
		}
		
		imageData = frameGray->imageData;

		
		//Passage en niveau de gris	
		RGBTOGRAY_1CANAL(frame, frameGray);

		
		//Sobel pour les gradients
		SobelHV(imageData,taille.height, taille.width, matriceGradX, matriceGradY);
		
		
		//Detecteur de Harris
		harris(matriceHarris, matriceGradX, matriceGradY, tailleInt);
	
		//Maxima de Harris
		getMaxima(matriceHarris, taille.height, taille.width, tabHarris, _NBHARRIS);
		
		calcul_vecteur_barycentre ( tabHarris, _NBHARRIS, &vecteurMoy, taille.height, taille.width);
				
		for(j=0;j<_NBHARRIS;j++)
  		{
			  tabHarrisPrec[j][0] = tabHarris[j][0];
			  tabHarrisPrec[j][1] = tabHarris[j][1];
			  tabHarrisPrec[j][2] = tabHarris[j][2];
		}
		
#ifdef DEBUGIA1		
		printf("nbVecteur match = %d \nveteur Moyen  = %d %d\n",nbVect,vecteurMoy.x,vecteurMoy.y);
		gettimeofday(&stop,NULL);
		timersub(&stop, &start, &res);
		printf("time Harris = %d %d\n",(int)res.tv_sec, (int)res.tv_usec);
#endif

		pilote_IA(vecteurMoy,1000,20,5, 255);
	}

	cvReleaseCapture( &capture );
#ifdef DEBUG2
printf("sortie converge odile.c\n");	
#endif
	return 0;
}

int prise_photo()
{
#ifdef DEBUG2
printf("entrer prise_photo odile.c\n");	
#endif

	struct timeval start,stop,res;

	open_capture(0,160,120);

	if ( !capture ) 
	{
		fprintf( stderr, "ERROR: capture is NULL \n" );
		getchar();
		return -1;
	}	

	//recuperation d une premiere image pour avoir les dimensions
	IplImage* frame = cvQueryFrame( capture );
	//recuperation des dimensions de l image
	CvSize taille = cvGetSize(frame);
	int tailleInt = taille.width * taille.height;
	
	//creation des image et matrices intermediaires

	IplImage* frameGray = cvCreateImage(taille,8,1);
	
	float* matriceGradX = malloc(sizeof(float)*tailleInt);
	float* matriceGradY = malloc(sizeof(float)*tailleInt);
	float* matriceHarris= malloc(sizeof(float)*tailleInt);
	float** tabHarris = (float **)malloc(sizeof(float*)*_NBHARRIS);
	char * imageData;
	char* buffer = (char*) malloc(sizeof(char)*20);
	int i=0,j;
	
	for(j=0;j<_NBHARRIS;j++)
    {
	   tabHarris[j]=(float*)malloc(sizeof(float)*3);	
    }

	while(i < 30)
	{
		
		//Recuperation de l image de la camera
		frame = cvQueryFrame( capture );
	
		if ( ! frame  )
		{
			fprintf( stderr, "ERROR: pas d'image\n" );
			return -1;
		}
		
		imageData = frameGray->imageData;

		gettimeofday(&start,NULL);
		//Passage en niveau de gris	
		RGBTOGRAY_1CANAL(frame, frameGray);
		
		//Sobel pour les gradients
		SobelHV(imageData,taille.height, taille.width, matriceGradX, matriceGradY);
				
		//Detecteur de Harris
		harris(matriceHarris, matriceGradX, matriceGradY, tailleInt);
	
		//Maxima de Harris
		getMaxima(matriceHarris, taille.height, taille.width, tabHarris, _NBHARRIS);
		for(j=0;j<_NBHARRIS;j++)
  		{
			
			Dessine_croix(imageData,taille.height, taille.width,tabHarris[j][1],tabHarris[j][0]);
		}		
		if(i > 19){
			sprintf(buffer,"image%d.jpg",i-19);
			cvSaveImage(buffer,frameGray,0);
		}
		
		gettimeofday(&stop,NULL);
		timersub(&stop, &start, &res);
		printf("time Harris = %i,%d sec\n",(int)res.tv_sec, (int)res.tv_usec);
	}
	
	cvReleaseCapture( &capture );
#ifdef DEBUG2
printf("sortie prise_photo odile.c\n");	
#endif
	return 0;
}
