//#include "pilotage.h"
//#include <curses.h>
#include "tools.h"
#include <stdlib.h>
#include <stdio.h>

int main(){

	/*
	 * TEST ALEX
	 * 
	 * 
    printf ("char : %d octets\n", sizeof(char));  
    printf ("int : %d octets\n", sizeof(int));  
    printf ("long : %d octets\n", sizeof(long));  
    printf ("double: %d octets\n", sizeof(double));

    int i=0,j=0;
    
    byte toto[2048];
    int size_toto = 10;
    byte tata[2048];
    int size_tata = 5;
    
    for( i = 0; i < size_toto;i++ ) 
    {
		toto[i]='a';
	}
	
	for( i = 0; i < size_tata;i++ ) 
    {
		tata[i]='b';
	}
	
	concatBytes(toto, size_toto, tata, size_tata);
	size_toto+=size_tata;
	
	for( i = 0; i < size_toto;i++ ) 
    {
		printf("%c\n", toto[i]);
	}
	
	cutBytes (toto, size_toto, 6);
	size_toto-=6;
	
	printf("debug\n");
	
	for( i = 0; i < size_toto;i++ ) 
    {
		printf("%c\n", toto[i]);
	}
	
	int tt = 123456789;
	byte tab[4];
	
	int test = convertIntToByte(tt, tab, 4);
	
	if (test = -1) printf("error\n");
	
	tt = convertByteToInt(tab, 0, 4);
	
	printf("%d\n", tt);

    exit(0);
    * 
    * FIN TEST ALEX
    * 
    */
    
    
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
