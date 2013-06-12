#ifndef _H_IMAGE_
#define _H_IMAGE_
#include "math.h"
#include </usr/include/opencv/cv.h>
#include </usr/include/opencv/highgui.h>
#define _NBHARRIS 10	
#define _SEUIL_DISTANCE 10

typedef struct {
	int x;
	int y;
}vecteur;

extern CvCapture* capture ;

void open_capture(int camera,int width,int height);

void SobelHV(char *buf,int myheight,int mywidth, float *bufHori, float *bufVert);

void harris( float* harris, float* gx, float* gy, int size );

void getMaxima( float* bufHarris, int myheight, int mywidth, float** tab, int nbPointsInterets );

int calcul_vecteur_interet(float ** pointsT, float ** pointsTplusUn, int nbPoints, int seuil,vecteur* resultat);
void calcul_moyenne_vecteur(vecteur* listeVecteurs, int tailleListeVecteur, vecteur* resultat );

void RGBTOGRAY_1CANAL(IplImage* source_couleur, IplImage* dest_gris);

void Dessine_croix(char *buf,int myheight,int mywidth,int ind_x, int ind_y);

void calcul_vecteur_barycentre ( float ** pointsT, int nbPoints, vecteur* resultat, int myheight,int mywidth);


#endif
