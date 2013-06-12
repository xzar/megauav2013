#include "odile.h"

void take_off(int altitude )
{
	int nick, roll, accNick, accRoll, alti,vario;
	signed char cmdNick = 0, cmdRoll = 0, cmdYaw = 0;
	int initNick, initRoll;
	unsigned char cmdGas = 255;
	unsigned char GasInter = 0;
	
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


void deplacement_zero(){


	struct timeval start,stop,res;
	open_capture(0,160,120);
	char* buffer = (char*) malloc(sizeof(char)*20);
	char * t2;
	//unsigned char * t3;
	if ( !capture ) 
	{
		fprintf( stderr, "ERROR: capture is NULL \n" );
		getchar();
		return -1;
	}

	printf("debut capture:\n");
	
	//recuperation d une premiere image pour avoir les dimensions
	//IplImage* frame = cvQueryFrame( capture );
	IplImage* frame = cvQueryFrame( capture );
	//recuperation des dimensions de l image
	CvSize taille = cvGetSize(frame);
	int tailleInt = taille.width * taille.height;
	//creation des image et matrices intermediaires
	IplImage* frameGray = cvCreateImage(taille,8,1);
	IplImage* frameGray2 = cvCreateImage(taille,8,1);
	float* matriceGradX = malloc(sizeof(float)*tailleInt);
	float* matriceGradY = malloc(sizeof(float)*tailleInt);
	float* matriceHarris= malloc(sizeof(float)*tailleInt);
	float** tabHarris = (float **)malloc(sizeof(float*)*_NBHARRIS);
	float** tabHarrisPrec = (float **)malloc(sizeof(float*)*_NBHARRIS);
	int j ,i = 0;
	char nick, roll, yaw;
	unsigned char gas =  255;
	int alti,vario;
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
	
	
	//int p[3];
	//cvNamedWindow( "toto", CV_WINDOW_AUTOSIZE );
	while(status == MODE_AUTO)
	{
		gettimeofday(&start,NULL);
		//Recuperation de l image de la camera
		//printf("toto 3\n");
		frame = cvQueryFrame( capture );
		printf("toto\n");
		//printf("toto 4\n");
		if ( ! frame  )
		{
			fprintf( stderr, "ERROR: pas d'image\n" );
			return 0;
		}
		
		t2 = frameGray->imageData;
		//t3 = frameGray2->imageData;
		//printf("toto 5\n");
		//printf("%d %d",taille.height,taille.width);		
		//Passage en niveau de gris
		//cvCvtColor(frame, frameGray, CV_RGB2GRAY);
		RGBTOGRAY_1CANAL(frame, frameGray);
		//printf("toto 6\n");
		
		//Sobel pour les gradients
		SobelHV(t2,taille.height, taille.width, matriceGradX, matriceGradY);
		//printf("sobel H te V : OK\n");
		
		//Detecteur de Harris
		harris(matriceHarris, matriceGradX, matriceGradY, tailleInt);
		//printf("harris : OK\n");
	

		//Maxima de Harris
		getMaxima(matriceHarris, taille.height, taille.width, tabHarris, _NBHARRIS);
		//printf("maxima : OK\n");
		
		
		//if(premierTour == 0){
			//Matching des points pour trouver les vecteurs
			nbVect = calcul_vecteur_interet(tabHarrisPrec, tabHarris, _NBHARRIS, _SEUIL_DISTANCE, tabVect);
			//printf("matching : OK nbvect %d\n",nbVect);
			
			//Moyenne des vecteurs
			calcul_moyenne_vecteur(tabVect, nbVect,&vecteurMoy);
			//printf("moyenne : OK\n");
			//premierTour=0;
		//}
	/*	}else{
			premierTour=0;	
		}*/
		printf("val harris : \n");
		for(j=0;j<_NBHARRIS;j++)
  		{
				//printf("j = %f %f\n",tabHarris[j][0],tabHarris[j][1]);
			  tabHarrisPrec[j][0] = tabHarris[j][0];
			  tabHarrisPrec[j][1] = tabHarris[j][1];
			  tabHarrisPrec[j][2] = tabHarris[j][2];
			
			//Dessine_croix(t,taille.height, taille.width,tabHarris[j][0],tabHarris[j][1]);
			//Dessine_croix(t2,taille.height, taille.width,tabHarris[j][1],tabHarris[j][0]);
			/*Dessine_croix(t2,taille.height,taille.width, 10,10);
			Dessine_croix(t2,taille.height,taille.width, 20,20);
			Dessine_croix(t2,taille.height,taille.width, 30,30);
			Dessine_croix(t2,taille.height,taille.width, 40,40);
			Dessine_croix(t2,taille.height, taille.width,0,50);*/	
   		}
		
		//printf("nbVecteur = %d \nveteur  = %d %d\n",nbVect,vecteurMoy.x,vecteurMoy.y);
		//printf("nbVecteur = %d %d\n",nick,roll);
		gettimeofday(&stop,NULL);
		timersub(&stop, &start, &res);
		printf("time = %i %d\n",res.tv_sec, res.tv_usec);
		//cvSaveImage("toto.jpg",frame,0);
		/*if(i > 19){
			sprintf(buffer,"toto%d.jpg",i);
			cvSaveImage(buffer,frameGray,0);
		}*/
		/*cvShowImage( "toto", frameGray );
		if ( (cvWaitKey(10) & 255) == 27 ) break;*/
		i++;
		if(vecteurMoy.y > 2){

			
			nick = (char)-10;
		}
		if(vecteurMoy.y < -2){
			nick = (char)10;
		}
		if(vecteurMoy.x > 2 ){

				roll = (char)-10;
		}
		
		if(vecteurMoy.x < -2){
			roll = (char)10;
		}
		/*alti = AnalogData[5];
		vario = AnalogData[6];
		if(gas < 180 && alti < 10){
			if(vario <= 1){
				gas++
			}else gas--;
		}else gas --;*/
	//	printf("puissance = %d %d\n",nick,roll);
		set_Nick( (signed char) nick );
		set_Roll( (signed char) roll );
		set_Yaw( (signed char) yaw ); 
		set_Gas( (unsigned char) gas );
		envoi_pilotage(file_mkusb);

		nick =0;
		roll =0;
		usleep(1000);

		set_Nick( (signed char) nick );
		set_Roll( (signed char) roll );
		set_Yaw( (signed char) yaw ); 
		set_Gas( (unsigned char) gas );
		envoi_pilotage(file_mkusb);

	}

	cvReleaseCapture( &capture );




}
