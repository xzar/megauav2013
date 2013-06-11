#include "image.h"


void SobelHV(char *buf,int myheight,int mywidth, float *bufHori, float *bufVert)
{
	int i,j;
	float sobelHo;
	float sobelVe;

	for (i = 1; i < myheight-1; i++)
    {
		for (j = 1; j < (mywidth)-1; j+=1)
    	{
			/*sobelHo =(buf[((i-1)*mywidth)+(j-1)]
				 -buf[((i-1)*mywidth)+(j+1)]
				 +buf[((i+1)*mywidth)+(j-1)]
				 -buf[((i+1)*mywidth)+(j+1)]
				 +2*buf[((i)*mywidth)+(j-1)]
				 -2*buf[((i)*mywidth)+(j+1)]);

			sobelVe =(buf[((i-1)*mywidth)+(j-1)]
				 +buf[((i-1)*mywidth)+(j+1)]
				 -buf[((i+1)*mywidth)+(j-1)]
				 -buf[((i+1)*mywidth)+(j+1)]
				 +2*buf[((i-1)*mywidth)+(j)]
				 -2*buf[((i+1)*mywidth)+(j)])/4;*/

			sobelHo =(buf[((i+1)*mywidth)+(j)]
				 -buf[((i-1)*mywidth)+(j)]) >> 1;

			sobelVe =(buf[((i)*mywidth)+(j-1)]
				 -buf[((i)*mywidth)+(j+1)]) >> 1;
			//printf("%d \n",sobelHo);
			bufHori[(i*mywidth)+(j)] = sobelHo ;
 			
			bufVert[(i*mywidth)+(j)] = sobelVe ;
			
		}
	}

}

void harris( float* harris, float* gx, float* gy, int size )
{
	int i;
	
	float gx2;
	float gy2;
	float gxy;

	
	
	
	for( i = 0; i < size; i++ )
	{
		// Calculer gx²
		gx2 = gx[i]*gx[i];
		
		// Calculer gy²
		gy2 = gy[i]*gy[i];
	
		// Calculer gx * gy
		gxy = gx[i]*gy[i];
		
		// Pour chaque pixel, calculer R
		harris[i] = (gx2 * gy2) - (gxy) - (0.04 * ( gx2 + gy2 ) * ( gx2 + gy2 ) ); 
		//printf("%f ",harris[i]);
	}

	
}

void getMaxima( float* bufHarris, int myheight, int mywidth, float** tab, int nbPointsInterets )
{
	int i,j,k;
	

   for(j=0;j<nbPointsInterets;j++)
   {
	   tab[j][0]=0;

	   tab[j][1]=0;
	   tab[j][2]=0;
   }

	for (i = 1; i < myheight-1; i++)
    { 
		for (j = 1; j < mywidth-1; j++)
    	{
			
					
					
					for( k = 0; k < nbPointsInterets; k++ )
					{
						if( bufHarris[i*mywidth+j] > tab[k][2] )
						{
							//printf("azeeaz\n");
							tab[k][0] = j;
							tab[k][1] = i;
							tab[k][2] = bufHarris[i*mywidth+j];
							break;
						}
					}
					
					/*if(indice == nbPointsInterets) indice--;
					
					if(indice > 0)
					{
						for( k = 0; k < indice; k++ )
						{
							tab[k][0] = tab[k+1][0];
							tab[k][1] = tab[k+1][1];
							tab[k][2] = tab[k+1][2];
						}
					//printf("coucou %d %d %f	 \n",i,j,bufHarris[i*mywidth+j]);
						tab[indice][0] = j;
						tab[indice][1] = i;
						tab[indice][2] = bufHarris[i*mywidth+j];	
					}*/
				
		}	
	}
	float t = tab[0][2] - tab[_NBHARRIS-1][2] ;
	 for( k = 0; k < nbPointsInterets; k++ )
	{

		tab[k][2] = (tab[k][2] /t)*255.0f;
		
	}
}

int calcul_vecteur_interet(float ** pointsT, float ** pointsTplusUn, int nbPoints, int seuil,vecteur* resultat){
	int it, itp;
	int compteVecteur = 0;
	int distanceHarris = 0;
	int premierPointTrouve = 0;
	vecteur v;
	
	//parcours des points a l'instant t
	for(it = 0; it < nbPoints; it++){
		distanceHarris =0;
		premierPointTrouve = 0;
		//parcours des points de l'instant tplusun
		for(itp = 0; itp < nbPoints; itp++){
			//printf("i = %d, j = %d\n",it,itp);
			//si point est proche dans le seuil
			if(	(pointsTplusUn[itp][0] < pointsT[it][0]+seuil)
				&& (pointsTplusUn[itp][0] > pointsT[it][0]-seuil)
				&& (pointsTplusUn[itp][1] < pointsT[it][1]+seuil)
				&&  (pointsTplusUn[itp][1] > pointsT[it][1]-seuil))
				{
					if (premierPointTrouve == 0){
						if(abs(pointsTplusUn[itp][2] - pointsT[it][2]) < 6){
							distanceHarris = (pointsTplusUn[itp][2] - pointsT[it][2]);
							v.x = pointsT[it][0] - pointsTplusUn[itp][0];
							v.y = pointsT[it][1] - pointsTplusUn[itp][1];
							
							premierPointTrouve =1;
							
						}
					}
					else{
						
						if ( abs(pointsTplusUn[itp][2] - pointsT[it][2]) < distanceHarris ){	
							distanceHarris = (pointsTplusUn[itp][2] - pointsT[it][2]);
							v.x = pointsT[it][0] - pointsTplusUn[itp][0];
							v.y = pointsT[it][1] - pointsTplusUn[itp][1];
							
						}
					}
					
				}
			}			
		if(premierPointTrouve != 0){
				resultat[compteVecteur]= v;
				compteVecteur ++;
		}
	}
	
	return compteVecteur;
}

void calcul_moyenne_vecteur(vecteur* listeVecteurs, int tailleListeVecteur, vecteur* resultat ){
		
		int index;
		int pileX = 0;
		int pileY = 0;
		for(index = 0; index < tailleListeVecteur; index ++){
				
			pileX +=listeVecteurs[index].x;
			pileY +=listeVecteurs[index].y;
			
		}
		
		if(tailleListeVecteur != 0){
			pileX = pileX / tailleListeVecteur;
			pileY = pileY / tailleListeVecteur;
		}
		
		resultat->x = pileX;
		resultat->y = pileY;
		
		
}

void RGBTOGRAY_1CANAL(IplImage* source_couleur, IplImage* dest_gris){
	int i,gris,size = dest_gris->width * dest_gris->height;
	unsigned char* data = (unsigned char*)source_couleur->imageData;
	for(i=0; i<size; i++)
	{
		gris = data[i*3 + 0];
		gris += data[i*3 + 1];
		gris += data[i*3 + 2];
		gris /= 3;
		dest_gris->imageData[i] = gris;
	}
}

void Dessine_croix(char *buf,int myheight,int mywidth,int ind_x, int ind_y)
{

	int i;

	for (i = -10; i <= 10; i++)
    	{ 
	 

			//printf("indice croix %d\n ", (((myheight/2)*3*mywidth)+(mywidth/2)*3));
			if((ind_x+i) > 0 && (ind_x+i) < myheight && (ind_y+i) > 0 && (ind_y+i) < mywidth){
				buf[(((ind_x)+i)*mywidth+(ind_y))]=255;

		
				buf[(((ind_x))*mywidth+((ind_y)+i))]=255;
			}
/*
			buf[(((myheight/2)+i)*3+((mywidth/2)+j)*3*myheight)+1]=0;
			buf[(((myheight/2)+i)*3+((mywidth/2)+j)*3*myheight)+2]=0;

			buf[(((myheight/2)-i)*3+((mywidth/2)+j)*3*myheight)]=255;
			buf[(((myheight/2)-i)*3+((mywidth/2)+j)*3*myheight)+1]=0;
			buf[(((myheight/2)-i)*3+((mywidth/2)+j)*3*myheight)+2]=0;

		*/
		
	}
	


}
