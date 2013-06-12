#include "image.h"


CvCapture* capture ;

void open_capture(int camera,int width,int height)
{
	capture = cvCaptureFromCAM( camera );
	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH, width );

	cvSetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT,  height	);


}



void SobelHV(char *buf,int myheight,int mywidth, float *bufHori, float *bufVert)
{
#ifdef DEBUG_IMAGE2
printf("entrer sobel image.c\n");	
#endif
	int i,j;
	float sobelHo;
	float sobelVe;

	for (i = 1; i < myheight-1; i++)
    {
		for (j = 1; j < (mywidth)-1; j+=1)
    	{
			
			sobelHo =(buf[((i+1)*mywidth)+(j)]
				 -buf[((i-1)*mywidth)+(j)]) >> 1;

			sobelVe =(buf[((i)*mywidth)+(j-1)]
				 -buf[((i)*mywidth)+(j+1)]) >> 1;
		
			bufHori[(i*mywidth)+(j)] = sobelHo ;
 			
			bufVert[(i*mywidth)+(j)] = sobelVe ;
			
		}
	}
#ifdef DEBUG_IMAGE2
printf("sorte sobel image.c\n");	
#endif
}

void harris( float* harris, float* gx, float* gy, int size )
{
#ifdef DEBUG_IMAGE2
printf("entrer harris image.c\n");	
#endif
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
		
	}

#ifdef DEBUG_IMAGE2
printf("sortie harris image.c\n");	
#endif
}

void getMaxima( float* bufHarris, int myheight, int mywidth, float** tab, int nbPointsInterets )
{
#ifdef DEBUG_IMAGE2
printf("entrer getMaxima image.c\n");	
#endif
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
							tab[k][0] = j;
							tab[k][1] = i;
							tab[k][2] = bufHarris[i*mywidth+j];
							break;
						}
					}
									
		}	
	}
	float t = tab[0][2] - tab[_NBHARRIS-1][2] ;
	 for( k = 0; k < nbPointsInterets; k++ )
	{

		tab[k][2] = (tab[k][2] /t)*255.0f;
		
	}
#ifdef DEBUG_IMAGE2
printf("sortie getMaxima image.c\n");	
#endif
}

int calcul_vecteur_interet(float ** pointsT, float ** pointsTplusUn, int nbPoints, int seuil,vecteur* resultat){
#ifdef DEBUG_IMAGE2
printf("entrer calcul_vecteur_interet image.c\n");	
#endif
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
#ifdef DEBUG_IMAGE2
printf("sortie calcul_vecteur_interet image.c\n");	
#endif
	return compteVecteur;
}

void calcul_moyenne_vecteur(vecteur* listeVecteurs, int tailleListeVecteur, vecteur* resultat ){
#ifdef DEBUG_IMAGE2
printf("entrer calcul_moyenne_vecteur image.c\n");	
#endif	
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
		
#ifdef DEBUG_IMAGE2
printf("sortie calcul_moyenne_vecteur image.c\n");	
#endif		
}

void RGBTOGRAY_1CANAL(IplImage* source_couleur, IplImage* dest_gris){
#ifdef DEBUG_IMAGE2
printf("entre RGBTOGRAY_1CANAL image.c\n");	
#endif
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
#ifdef DEBUG_IMAGE2
printf("sortie RGBTOGRAY_1CANAL image.c\n");	
#endif
}

void Dessine_croix(char *buf,int myheight,int mywidth,int ind_x, int ind_y)
{
#ifdef DEBUG_IMAGE2
printf("entre dessine_croix image.c\n");	
#endif

	int i;

	for (i = -10; i <= 10; i++)
    	{
			if((ind_x+i) > 0 && (ind_x+i) < myheight && (ind_y+i) > 0 && (ind_y+i) < mywidth){
				
				buf[(((ind_x)+i)*mywidth+(ind_y))]=255;
		
				buf[(((ind_x))*mywidth+((ind_y)+i))]=255;
			}

		
	}
	
#ifdef DEBUG_IMAGE2
printf("sortie dessine_croix image.c\n");	
#endif
}

void calcul_vecteur_barycentre ( float ** pointsT, int nbPoints, vecteur* resultat, int myheight,int mywidth){
#ifdef DEBUG_IMAGE2
printf("entre calcul_vecteur_barycentre image.c\n");	
#endif
	int it;
	int baryX = 0;
	int baryY = 0;
	resultat->x = 0;
	resultat->y = 0;
	//parcours des points
	for(it = 0; it < nbPoints; it++){
		baryX += pointsT[it][0];
		baryY += pointsT[it][1];
	}
	//calcul du barycentre
	baryX = baryX / nbPoints;
	baryY = baryY / nbPoints;
	
	//calcul vecteur par rapport au centre
	resultat->x = baryX-(mywidth/2);
	resultat->y = baryY-(myheight/2);
#ifdef DEBUG_IMAGE2
printf("sorite calcul_vecteur_barycentre image.c\n");	
#endif
}

void fast_valeur(char* image, int myheight, int mywidth, float** maxHarris, int nbPointsInterets, int seuil)
{
#ifdef DEBUG_IMAGE2
printf("entre fast_Valeur image.c\n");	
#endif
	int compteur;
	int pixel_a_plat[16];
	//pour chaque coin max detecte par harris
	for(compteur = 0; compteur < nbPointsInterets; compteur ++)
	{
		//si le point n est pas dans les bordures
		if( 
			maxHarris[compteur][0] > 3
			&& maxHarris[compteur][0] < mywidth - 3
			&& maxHarris[compteur][1] > 3
			&& maxHarris[compteur][1] < myheight - 3)
		{
			mise_a_plat(image, pixel_a_plat,maxHarris[compteur][0],maxHarris[compteur][1], mywidth);
			maxHarris[compteur][3] = fast9(pixel_a_plat, seuil,image[ ((int)maxHarris[compteur][0]*mywidth+(int)maxHarris[compteur][1])] );
		}
		
		else
		maxHarris[compteur][3] = -1;
	}
#ifdef DEBUG_IMAGE2
printf("sortie fast valeur image.c\n");	
#endif
}

void mise_a_plat(char* image, int* pixel,int x, int y, int mywidth)
{
#ifdef DEBUG_IMAGE2
printf("entre mise a plat image.c\n");	
#endif
        pixel[0] = image[x + 0 + mywidth * 3 + mywidth * y];
        pixel[1] = image[x + 1 + mywidth * 3 + mywidth * y];
        pixel[2] = image[x + 2 + mywidth * 2 + mywidth * y];
        pixel[3] = image[x + 3 + mywidth * 1 + mywidth * y];
        pixel[4] = image[x + 3 + mywidth * 0 + mywidth * y];
        pixel[5] = image[x + 3 + mywidth * -1 + mywidth * y];
        pixel[6] = image[x + 2 + mywidth * -2 + mywidth * y];
        pixel[7] = image[x + 1 + mywidth * -3 + mywidth * y];
        pixel[8] = image[x + 0 + mywidth * -3 + mywidth * y];
        pixel[9] = image[x + -1 + mywidth * -3 + mywidth * y];
        pixel[10] = image[x + -2 + mywidth * -2 + mywidth * y];
        pixel[11] = image[x + -3 + mywidth * -1 + mywidth * y];
        pixel[12] = image[x + -3 + mywidth * 0 + mywidth * y];
        pixel[13] = image[x + -3 + mywidth * 1 + mywidth * y];
        pixel[14] = image[x + -2 + mywidth * 2 + mywidth * y];
        pixel[15] = image[x + -1 + mywidth * 3 + mywidth * y];
#ifdef DEBUG_IMAGE2
printf("sortie mise a plat image.c\n");	
#endif
}

int fast9(int* pixel, int bstart, int p)
{  
#ifdef DEBUG_IMAGE2
printf("entre fast9 image.c\n");	
#endif  
    int bmin = bstart;
    int bmax = 255;
    int b = (bmax + bmin)/2;
    
    /*Compute the score using binary search*/
	for(;;)
    {
		int cb = p + b;
		int c_b= p - b;


        if( pixel[0] > cb)
         if( pixel[1] > cb)
          if( pixel[2] > cb)
           if( pixel[3] > cb)
            if( pixel[4] > cb)
             if( pixel[5] > cb)
              if( pixel[6] > cb)
               if( pixel[7] > cb)
                if( pixel[8] > cb)
                 goto is_a_corner;
                else
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
               else if( pixel[7] < c_b)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else if( pixel[14] < c_b)
                 if( pixel[8] < c_b)
                  if( pixel[9] < c_b)
                   if( pixel[10] < c_b)
                    if( pixel[11] < c_b)
                     if( pixel[12] < c_b)
                      if( pixel[13] < c_b)
                       if( pixel[15] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else if( pixel[6] < c_b)
               if( pixel[15] > cb)
                if( pixel[13] > cb)
                 if( pixel[14] > cb)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else if( pixel[13] < c_b)
                 if( pixel[7] < c_b)
                  if( pixel[8] < c_b)
                   if( pixel[9] < c_b)
                    if( pixel[10] < c_b)
                     if( pixel[11] < c_b)
                      if( pixel[12] < c_b)
                       if( pixel[14] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                if( pixel[7] < c_b)
                 if( pixel[8] < c_b)
                  if( pixel[9] < c_b)
                   if( pixel[10] < c_b)
                    if( pixel[11] < c_b)
                     if( pixel[12] < c_b)
                      if( pixel[13] < c_b)
                       if( pixel[14] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else if( pixel[13] < c_b)
                if( pixel[7] < c_b)
                 if( pixel[8] < c_b)
                  if( pixel[9] < c_b)
                   if( pixel[10] < c_b)
                    if( pixel[11] < c_b)
                     if( pixel[12] < c_b)
                      if( pixel[14] < c_b)
                       if( pixel[15] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else if( pixel[5] < c_b)
              if( pixel[14] > cb)
               if( pixel[12] > cb)
                if( pixel[13] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      if( pixel[10] > cb)
                       if( pixel[11] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else if( pixel[12] < c_b)
                if( pixel[6] < c_b)
                 if( pixel[7] < c_b)
                  if( pixel[8] < c_b)
                   if( pixel[9] < c_b)
                    if( pixel[10] < c_b)
                     if( pixel[11] < c_b)
                      if( pixel[13] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else if( pixel[14] < c_b)
               if( pixel[7] < c_b)
                if( pixel[8] < c_b)
                 if( pixel[9] < c_b)
                  if( pixel[10] < c_b)
                   if( pixel[11] < c_b)
                    if( pixel[12] < c_b)
                     if( pixel[13] < c_b)
                      if( pixel[6] < c_b)
                       goto is_a_corner;
                      else
                       if( pixel[15] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               if( pixel[6] < c_b)
                if( pixel[7] < c_b)
                 if( pixel[8] < c_b)
                  if( pixel[9] < c_b)
                   if( pixel[10] < c_b)
                    if( pixel[11] < c_b)
                     if( pixel[12] < c_b)
                      if( pixel[13] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      if( pixel[10] > cb)
                       if( pixel[11] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else if( pixel[12] < c_b)
               if( pixel[7] < c_b)
                if( pixel[8] < c_b)
                 if( pixel[9] < c_b)
                  if( pixel[10] < c_b)
                   if( pixel[11] < c_b)
                    if( pixel[13] < c_b)
                     if( pixel[14] < c_b)
                      if( pixel[6] < c_b)
                       goto is_a_corner;
                      else
                       if( pixel[15] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else if( pixel[4] < c_b)
             if( pixel[13] > cb)
              if( pixel[11] > cb)
               if( pixel[12] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      if( pixel[10] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      if( pixel[10] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else if( pixel[11] < c_b)
               if( pixel[5] < c_b)
                if( pixel[6] < c_b)
                 if( pixel[7] < c_b)
                  if( pixel[8] < c_b)
                   if( pixel[9] < c_b)
                    if( pixel[10] < c_b)
                     if( pixel[12] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else if( pixel[13] < c_b)
              if( pixel[7] < c_b)
               if( pixel[8] < c_b)
                if( pixel[9] < c_b)
                 if( pixel[10] < c_b)
                  if( pixel[11] < c_b)
                   if( pixel[12] < c_b)
                    if( pixel[6] < c_b)
                     if( pixel[5] < c_b)
                      goto is_a_corner;
                     else
                      if( pixel[14] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                    else
                     if( pixel[14] < c_b)
                      if( pixel[15] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              if( pixel[5] < c_b)
               if( pixel[6] < c_b)
                if( pixel[7] < c_b)
                 if( pixel[8] < c_b)
                  if( pixel[9] < c_b)
                   if( pixel[10] < c_b)
                    if( pixel[11] < c_b)
                     if( pixel[12] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             if( pixel[11] > cb)
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      if( pixel[10] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      if( pixel[10] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else if( pixel[11] < c_b)
              if( pixel[7] < c_b)
               if( pixel[8] < c_b)
                if( pixel[9] < c_b)
                 if( pixel[10] < c_b)
                  if( pixel[12] < c_b)
                   if( pixel[13] < c_b)
                    if( pixel[6] < c_b)
                     if( pixel[5] < c_b)
                      goto is_a_corner;
                     else
                      if( pixel[14] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                    else
                     if( pixel[14] < c_b)
                      if( pixel[15] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
           else if( pixel[3] < c_b)
            if( pixel[10] > cb)
             if( pixel[11] > cb)
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else if( pixel[10] < c_b)
             if( pixel[7] < c_b)
              if( pixel[8] < c_b)
               if( pixel[9] < c_b)
                if( pixel[11] < c_b)
                 if( pixel[6] < c_b)
                  if( pixel[5] < c_b)
                   if( pixel[4] < c_b)
                    goto is_a_corner;
                   else
                    if( pixel[12] < c_b)
                     if( pixel[13] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                  else
                   if( pixel[12] < c_b)
                    if( pixel[13] < c_b)
                     if( pixel[14] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[12] < c_b)
                   if( pixel[13] < c_b)
                    if( pixel[14] < c_b)
                     if( pixel[15] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            if( pixel[10] > cb)
             if( pixel[11] > cb)
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     if( pixel[9] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else if( pixel[10] < c_b)
             if( pixel[7] < c_b)
              if( pixel[8] < c_b)
               if( pixel[9] < c_b)
                if( pixel[11] < c_b)
                 if( pixel[12] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[5] < c_b)
                    if( pixel[4] < c_b)
                     goto is_a_corner;
                    else
                     if( pixel[13] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                   else
                    if( pixel[13] < c_b)
                     if( pixel[14] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                  else
                   if( pixel[13] < c_b)
                    if( pixel[14] < c_b)
                     if( pixel[15] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
          else if( pixel[2] < c_b)
           if( pixel[9] > cb)
            if( pixel[10] > cb)
             if( pixel[11] > cb)
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] > cb)
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else if( pixel[9] < c_b)
            if( pixel[7] < c_b)
             if( pixel[8] < c_b)
              if( pixel[10] < c_b)
               if( pixel[6] < c_b)
                if( pixel[5] < c_b)
                 if( pixel[4] < c_b)
                  if( pixel[3] < c_b)
                   goto is_a_corner;
                  else
                   if( pixel[11] < c_b)
                    if( pixel[12] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[11] < c_b)
                   if( pixel[12] < c_b)
                    if( pixel[13] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[11] < c_b)
                  if( pixel[12] < c_b)
                   if( pixel[13] < c_b)
                    if( pixel[14] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[11] < c_b)
                 if( pixel[12] < c_b)
                  if( pixel[13] < c_b)
                   if( pixel[14] < c_b)
                    if( pixel[15] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else
           if( pixel[9] > cb)
            if( pixel[10] > cb)
             if( pixel[11] > cb)
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] > cb)
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    if( pixel[8] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else if( pixel[9] < c_b)
            if( pixel[7] < c_b)
             if( pixel[8] < c_b)
              if( pixel[10] < c_b)
               if( pixel[11] < c_b)
                if( pixel[6] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[4] < c_b)
                   if( pixel[3] < c_b)
                    goto is_a_corner;
                   else
                    if( pixel[12] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                  else
                   if( pixel[12] < c_b)
                    if( pixel[13] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[12] < c_b)
                   if( pixel[13] < c_b)
                    if( pixel[14] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[12] < c_b)
                  if( pixel[13] < c_b)
                   if( pixel[14] < c_b)
                    if( pixel[15] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
         else if( pixel[1] < c_b)
          if( pixel[8] > cb)
           if( pixel[9] > cb)
            if( pixel[10] > cb)
             if( pixel[11] > cb)
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] > cb)
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[2] > cb)
               if( pixel[3] > cb)
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else if( pixel[8] < c_b)
           if( pixel[7] < c_b)
            if( pixel[9] < c_b)
             if( pixel[6] < c_b)
              if( pixel[5] < c_b)
               if( pixel[4] < c_b)
                if( pixel[3] < c_b)
                 if( pixel[2] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[10] < c_b)
                   if( pixel[11] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[10] < c_b)
                  if( pixel[11] < c_b)
                   if( pixel[12] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[10] < c_b)
                 if( pixel[11] < c_b)
                  if( pixel[12] < c_b)
                   if( pixel[13] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[10] < c_b)
                if( pixel[11] < c_b)
                 if( pixel[12] < c_b)
                  if( pixel[13] < c_b)
                   if( pixel[14] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[10] < c_b)
               if( pixel[11] < c_b)
                if( pixel[12] < c_b)
                 if( pixel[13] < c_b)
                  if( pixel[14] < c_b)
                   if( pixel[15] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else
           goto is_not_a_corner;
         else
          if( pixel[8] > cb)
           if( pixel[9] > cb)
            if( pixel[10] > cb)
             if( pixel[11] > cb)
              if( pixel[12] > cb)
               if( pixel[13] > cb)
                if( pixel[14] > cb)
                 if( pixel[15] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] > cb)
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[2] > cb)
               if( pixel[3] > cb)
                if( pixel[4] > cb)
                 if( pixel[5] > cb)
                  if( pixel[6] > cb)
                   if( pixel[7] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else if( pixel[8] < c_b)
           if( pixel[7] < c_b)
            if( pixel[9] < c_b)
             if( pixel[10] < c_b)
              if( pixel[6] < c_b)
               if( pixel[5] < c_b)
                if( pixel[4] < c_b)
                 if( pixel[3] < c_b)
                  if( pixel[2] < c_b)
                   goto is_a_corner;
                  else
                   if( pixel[11] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[11] < c_b)
                   if( pixel[12] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[11] < c_b)
                  if( pixel[12] < c_b)
                   if( pixel[13] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[11] < c_b)
                 if( pixel[12] < c_b)
                  if( pixel[13] < c_b)
                   if( pixel[14] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[11] < c_b)
                if( pixel[12] < c_b)
                 if( pixel[13] < c_b)
                  if( pixel[14] < c_b)
                   if( pixel[15] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else
           goto is_not_a_corner;
        else if( pixel[0] < c_b)
         if( pixel[1] > cb)
          if( pixel[8] > cb)
           if( pixel[7] > cb)
            if( pixel[9] > cb)
             if( pixel[6] > cb)
              if( pixel[5] > cb)
               if( pixel[4] > cb)
                if( pixel[3] > cb)
                 if( pixel[2] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[10] > cb)
                   if( pixel[11] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[10] > cb)
                  if( pixel[11] > cb)
                   if( pixel[12] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[10] > cb)
                 if( pixel[11] > cb)
                  if( pixel[12] > cb)
                   if( pixel[13] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[10] > cb)
                if( pixel[11] > cb)
                 if( pixel[12] > cb)
                  if( pixel[13] > cb)
                   if( pixel[14] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[10] > cb)
               if( pixel[11] > cb)
                if( pixel[12] > cb)
                 if( pixel[13] > cb)
                  if( pixel[14] > cb)
                   if( pixel[15] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else if( pixel[8] < c_b)
           if( pixel[9] < c_b)
            if( pixel[10] < c_b)
             if( pixel[11] < c_b)
              if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] < c_b)
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[2] < c_b)
               if( pixel[3] < c_b)
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else
           goto is_not_a_corner;
         else if( pixel[1] < c_b)
          if( pixel[2] > cb)
           if( pixel[9] > cb)
            if( pixel[7] > cb)
             if( pixel[8] > cb)
              if( pixel[10] > cb)
               if( pixel[6] > cb)
                if( pixel[5] > cb)
                 if( pixel[4] > cb)
                  if( pixel[3] > cb)
                   goto is_a_corner;
                  else
                   if( pixel[11] > cb)
                    if( pixel[12] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[11] > cb)
                   if( pixel[12] > cb)
                    if( pixel[13] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[11] > cb)
                  if( pixel[12] > cb)
                   if( pixel[13] > cb)
                    if( pixel[14] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[11] > cb)
                 if( pixel[12] > cb)
                  if( pixel[13] > cb)
                   if( pixel[14] > cb)
                    if( pixel[15] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else if( pixel[9] < c_b)
            if( pixel[10] < c_b)
             if( pixel[11] < c_b)
              if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] < c_b)
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else if( pixel[2] < c_b)
           if( pixel[3] > cb)
            if( pixel[10] > cb)
             if( pixel[7] > cb)
              if( pixel[8] > cb)
               if( pixel[9] > cb)
                if( pixel[11] > cb)
                 if( pixel[6] > cb)
                  if( pixel[5] > cb)
                   if( pixel[4] > cb)
                    goto is_a_corner;
                   else
                    if( pixel[12] > cb)
                     if( pixel[13] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                  else
                   if( pixel[12] > cb)
                    if( pixel[13] > cb)
                     if( pixel[14] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[12] > cb)
                   if( pixel[13] > cb)
                    if( pixel[14] > cb)
                     if( pixel[15] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else if( pixel[10] < c_b)
             if( pixel[11] < c_b)
              if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else if( pixel[3] < c_b)
            if( pixel[4] > cb)
             if( pixel[13] > cb)
              if( pixel[7] > cb)
               if( pixel[8] > cb)
                if( pixel[9] > cb)
                 if( pixel[10] > cb)
                  if( pixel[11] > cb)
                   if( pixel[12] > cb)
                    if( pixel[6] > cb)
                     if( pixel[5] > cb)
                      goto is_a_corner;
                     else
                      if( pixel[14] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                    else
                     if( pixel[14] > cb)
                      if( pixel[15] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else if( pixel[13] < c_b)
              if( pixel[11] > cb)
               if( pixel[5] > cb)
                if( pixel[6] > cb)
                 if( pixel[7] > cb)
                  if( pixel[8] > cb)
                   if( pixel[9] > cb)
                    if( pixel[10] > cb)
                     if( pixel[12] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else if( pixel[11] < c_b)
               if( pixel[12] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      if( pixel[10] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      if( pixel[10] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              if( pixel[5] > cb)
               if( pixel[6] > cb)
                if( pixel[7] > cb)
                 if( pixel[8] > cb)
                  if( pixel[9] > cb)
                   if( pixel[10] > cb)
                    if( pixel[11] > cb)
                     if( pixel[12] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else if( pixel[4] < c_b)
             if( pixel[5] > cb)
              if( pixel[14] > cb)
               if( pixel[7] > cb)
                if( pixel[8] > cb)
                 if( pixel[9] > cb)
                  if( pixel[10] > cb)
                   if( pixel[11] > cb)
                    if( pixel[12] > cb)
                     if( pixel[13] > cb)
                      if( pixel[6] > cb)
                       goto is_a_corner;
                      else
                       if( pixel[15] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else if( pixel[14] < c_b)
               if( pixel[12] > cb)
                if( pixel[6] > cb)
                 if( pixel[7] > cb)
                  if( pixel[8] > cb)
                   if( pixel[9] > cb)
                    if( pixel[10] > cb)
                     if( pixel[11] > cb)
                      if( pixel[13] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else if( pixel[12] < c_b)
                if( pixel[13] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      if( pixel[10] < c_b)
                       if( pixel[11] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               if( pixel[6] > cb)
                if( pixel[7] > cb)
                 if( pixel[8] > cb)
                  if( pixel[9] > cb)
                   if( pixel[10] > cb)
                    if( pixel[11] > cb)
                     if( pixel[12] > cb)
                      if( pixel[13] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else if( pixel[5] < c_b)
              if( pixel[6] > cb)
               if( pixel[15] < c_b)
                if( pixel[13] > cb)
                 if( pixel[7] > cb)
                  if( pixel[8] > cb)
                   if( pixel[9] > cb)
                    if( pixel[10] > cb)
                     if( pixel[11] > cb)
                      if( pixel[12] > cb)
                       if( pixel[14] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else if( pixel[13] < c_b)
                 if( pixel[14] < c_b)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                if( pixel[7] > cb)
                 if( pixel[8] > cb)
                  if( pixel[9] > cb)
                   if( pixel[10] > cb)
                    if( pixel[11] > cb)
                     if( pixel[12] > cb)
                      if( pixel[13] > cb)
                       if( pixel[14] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else if( pixel[6] < c_b)
               if( pixel[7] > cb)
                if( pixel[14] > cb)
                 if( pixel[8] > cb)
                  if( pixel[9] > cb)
                   if( pixel[10] > cb)
                    if( pixel[11] > cb)
                     if( pixel[12] > cb)
                      if( pixel[13] > cb)
                       if( pixel[15] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else if( pixel[7] < c_b)
                if( pixel[8] < c_b)
                 goto is_a_corner;
                else
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[13] > cb)
                if( pixel[7] > cb)
                 if( pixel[8] > cb)
                  if( pixel[9] > cb)
                   if( pixel[10] > cb)
                    if( pixel[11] > cb)
                     if( pixel[12] > cb)
                      if( pixel[14] > cb)
                       if( pixel[15] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[12] > cb)
               if( pixel[7] > cb)
                if( pixel[8] > cb)
                 if( pixel[9] > cb)
                  if( pixel[10] > cb)
                   if( pixel[11] > cb)
                    if( pixel[13] > cb)
                     if( pixel[14] > cb)
                      if( pixel[6] > cb)
                       goto is_a_corner;
                      else
                       if( pixel[15] > cb)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      if( pixel[10] < c_b)
                       if( pixel[11] < c_b)
                        goto is_a_corner;
                       else
                        goto is_not_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             if( pixel[11] > cb)
              if( pixel[7] > cb)
               if( pixel[8] > cb)
                if( pixel[9] > cb)
                 if( pixel[10] > cb)
                  if( pixel[12] > cb)
                   if( pixel[13] > cb)
                    if( pixel[6] > cb)
                     if( pixel[5] > cb)
                      goto is_a_corner;
                     else
                      if( pixel[14] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                    else
                     if( pixel[14] > cb)
                      if( pixel[15] > cb)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else if( pixel[11] < c_b)
              if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      if( pixel[10] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      if( pixel[10] < c_b)
                       goto is_a_corner;
                      else
                       goto is_not_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
           else
            if( pixel[10] > cb)
             if( pixel[7] > cb)
              if( pixel[8] > cb)
               if( pixel[9] > cb)
                if( pixel[11] > cb)
                 if( pixel[12] > cb)
                  if( pixel[6] > cb)
                   if( pixel[5] > cb)
                    if( pixel[4] > cb)
                     goto is_a_corner;
                    else
                     if( pixel[13] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                   else
                    if( pixel[13] > cb)
                     if( pixel[14] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                  else
                   if( pixel[13] > cb)
                    if( pixel[14] > cb)
                     if( pixel[15] > cb)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else if( pixel[10] < c_b)
             if( pixel[11] < c_b)
              if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     if( pixel[9] < c_b)
                      goto is_a_corner;
                     else
                      goto is_not_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
          else
           if( pixel[9] > cb)
            if( pixel[7] > cb)
             if( pixel[8] > cb)
              if( pixel[10] > cb)
               if( pixel[11] > cb)
                if( pixel[6] > cb)
                 if( pixel[5] > cb)
                  if( pixel[4] > cb)
                   if( pixel[3] > cb)
                    goto is_a_corner;
                   else
                    if( pixel[12] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                  else
                   if( pixel[12] > cb)
                    if( pixel[13] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[12] > cb)
                   if( pixel[13] > cb)
                    if( pixel[14] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[12] > cb)
                  if( pixel[13] > cb)
                   if( pixel[14] > cb)
                    if( pixel[15] > cb)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else if( pixel[9] < c_b)
            if( pixel[10] < c_b)
             if( pixel[11] < c_b)
              if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] < c_b)
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    if( pixel[8] < c_b)
                     goto is_a_corner;
                    else
                     goto is_not_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
         else
          if( pixel[8] > cb)
           if( pixel[7] > cb)
            if( pixel[9] > cb)
             if( pixel[10] > cb)
              if( pixel[6] > cb)
               if( pixel[5] > cb)
                if( pixel[4] > cb)
                 if( pixel[3] > cb)
                  if( pixel[2] > cb)
                   goto is_a_corner;
                  else
                   if( pixel[11] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                 else
                  if( pixel[11] > cb)
                   if( pixel[12] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[11] > cb)
                  if( pixel[12] > cb)
                   if( pixel[13] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[11] > cb)
                 if( pixel[12] > cb)
                  if( pixel[13] > cb)
                   if( pixel[14] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[11] > cb)
                if( pixel[12] > cb)
                 if( pixel[13] > cb)
                  if( pixel[14] > cb)
                   if( pixel[15] > cb)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else if( pixel[8] < c_b)
           if( pixel[9] < c_b)
            if( pixel[10] < c_b)
             if( pixel[11] < c_b)
              if( pixel[12] < c_b)
               if( pixel[13] < c_b)
                if( pixel[14] < c_b)
                 if( pixel[15] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[3] < c_b)
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[2] < c_b)
               if( pixel[3] < c_b)
                if( pixel[4] < c_b)
                 if( pixel[5] < c_b)
                  if( pixel[6] < c_b)
                   if( pixel[7] < c_b)
                    goto is_a_corner;
                   else
                    goto is_not_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else
           goto is_not_a_corner;
        else
         if( pixel[7] > cb)
          if( pixel[8] > cb)
           if( pixel[9] > cb)
            if( pixel[6] > cb)
             if( pixel[5] > cb)
              if( pixel[4] > cb)
               if( pixel[3] > cb)
                if( pixel[2] > cb)
                 if( pixel[1] > cb)
                  goto is_a_corner;
                 else
                  if( pixel[10] > cb)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[10] > cb)
                  if( pixel[11] > cb)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[10] > cb)
                 if( pixel[11] > cb)
                  if( pixel[12] > cb)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[10] > cb)
                if( pixel[11] > cb)
                 if( pixel[12] > cb)
                  if( pixel[13] > cb)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[10] > cb)
               if( pixel[11] > cb)
                if( pixel[12] > cb)
                 if( pixel[13] > cb)
                  if( pixel[14] > cb)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             if( pixel[10] > cb)
              if( pixel[11] > cb)
               if( pixel[12] > cb)
                if( pixel[13] > cb)
                 if( pixel[14] > cb)
                  if( pixel[15] > cb)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else
           goto is_not_a_corner;
         else if( pixel[7] < c_b)
          if( pixel[8] < c_b)
           if( pixel[9] < c_b)
            if( pixel[6] < c_b)
             if( pixel[5] < c_b)
              if( pixel[4] < c_b)
               if( pixel[3] < c_b)
                if( pixel[2] < c_b)
                 if( pixel[1] < c_b)
                  goto is_a_corner;
                 else
                  if( pixel[10] < c_b)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                else
                 if( pixel[10] < c_b)
                  if( pixel[11] < c_b)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
               else
                if( pixel[10] < c_b)
                 if( pixel[11] < c_b)
                  if( pixel[12] < c_b)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
              else
               if( pixel[10] < c_b)
                if( pixel[11] < c_b)
                 if( pixel[12] < c_b)
                  if( pixel[13] < c_b)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
             else
              if( pixel[10] < c_b)
               if( pixel[11] < c_b)
                if( pixel[12] < c_b)
                 if( pixel[13] < c_b)
                  if( pixel[14] < c_b)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
            else
             if( pixel[10] < c_b)
              if( pixel[11] < c_b)
               if( pixel[12] < c_b)
                if( pixel[13] < c_b)
                 if( pixel[14] < c_b)
                  if( pixel[15] < c_b)
                   goto is_a_corner;
                  else
                   goto is_not_a_corner;
                 else
                  goto is_not_a_corner;
                else
                 goto is_not_a_corner;
               else
                goto is_not_a_corner;
              else
               goto is_not_a_corner;
             else
              goto is_not_a_corner;
           else
            goto is_not_a_corner;
          else
           goto is_not_a_corner;
         else
          goto is_not_a_corner;

		is_a_corner:
			bmin=b;
			goto end_if;

		is_not_a_corner:
			bmax=b;
			goto end_if;

		end_if:

		if(bmin == bmax - 1 || bmin == bmax)
			return bmin;
		b = (bmin + bmax) / 2;
    }
#ifdef DEBUG_IMAGE2
printf("entre fast9 image.c\n");	
#endif
}

