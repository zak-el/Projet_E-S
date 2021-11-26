#include "stdes.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	FICHIER *f1;
	FICHIER *f2;
	char c;
	char s[1000] ;
	
	fstdout = (FICHIER*)malloc(sizeof(FICHIER));
	fstdout->fd = 1 ;
	fstdout->place = 0 ;
	fstdout->taille = SIZE ;
	fstdout->mode = 'E' ;
	
	
	if (argc != 3)
		exit(-1);

	f1 = ouvrir (argv[1], 'L');
	if (f1 == NULL)
		exit (-1);

	f2 = ouvrir (argv[2], 'E');
	if (f2 == NULL)
		exit (-1);
  int n ;
	while ((n = lire (s, 1, 11, f1) )!= 0) {
          //ecrire (s, 1, 11, fstdout);
          //ecrire (&c, 1, 1, f2);
          printf(" n == %d\n", n);
	}

   //int v = lire (s, 1, 2, f1) ;
   //ecrire (s, 1, 2, fstdout);
  //printf("kkkk %d kkkkkk", v) ;



  
        vider (fstdout);

        fermer (f1);
        fermer (f2);

	return 0;
}

