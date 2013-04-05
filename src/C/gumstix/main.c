//#include "pilotage.h"
//#include <curses.h>
#include "tools.h"
#include <stdlib.h>
#include <stdio.h>

int main(){

    printf ("char : %d octets\n", sizeof(char));  
    printf ("int : %d octets\n", sizeof(int));  
    printf ("long : %d octets\n", sizeof(long));  
    printf ("double: %d octets\n", sizeof(double));

    int tab[20];
    
    int i=0, j=0;
    byte * toto;
    int size_toto = 10;
    byte *tata;
    int size_tata=5;
    
    printf("debug\n");
    
    toto = malloc( size_toto );
    tata = malloc( size_tata * sizeof(byte) );

    printf("addr toto 1:%d\n", toto);
    
    printf("debug\n");
    
    for (i = 0; i < size_toto; i++)
    {
        toto[i]='a';
        //printf("toto%c\n", toto[i]);
    }

    for (i = 0; i < size_tata; i++)
    {
        tata[i]='b';
        //printf("%c\n", tata[i]);
    }
    
    printf("debug\n");

    
    concatBytes(toto, size_toto, tata, size_tata);
    printf("addr toto2:%d\n", toto);
    size_toto += size_tata;

    cutBytes(toto, &size_toto, 10);
    
    for (i = 0; i < size_toto; i++)
    {
        printf("%d:%c\n", i,toto[i]);
    }
    printf("addr toto3:%d\n", toto);

    free(toto);
    free(tata);

    exit(0);
    /*
	int file;
	char* buf_reseau = (char *) malloc(20*sizeof(char));
	//struct pour le control
	

	//structure pour la reception du debug
	//struct str_DebugOut DebugOut;

	//ouverture du port série
	serial_open(&file, "/dev/ttyUSB0");
	
	
	//envoi requete pour le debug
	
	//SendOutData('d', 0, buf2, 0, file);

	//init reseau
	open_reseau();



	memset(&ExternControl, 0, sizeof(struct str_ExternControl) );

	init_pilotage();

	while(1){
		
		

			attente_reseau(buf_reseau, 4);
			printf("ca passe apres\n");
			set_Nick( (signed char) buf_reseau[0] );
			set_Roll( (signed char) buf_reseau[1] );
			set_Yaw( (signed char) buf_reseau[2] ); 
			set_Gas( (unsigned char) buf_reseau[3] ); 
			
			printf("bufreseau = %s \n ",buf_reseau);
			 //envoi_pilotage(file);
	}
	
		
		
	
//	close_reseau();
	close(file);
	free(buf_reseau);
    */
}  
