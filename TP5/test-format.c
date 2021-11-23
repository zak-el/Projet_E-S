#include <unistd.h>
#include <stdlib.h>
#include "stdes.h"
#include <stdio.h>
#include <string.h>


int main (int argc, char **argv)
{

	fstdout = (FICHIER*)malloc(sizeof(FICHIER));
	fstdout->fd = 1 ;
	fstdout->place = 0 ;
	fstdout->taille = SIZE ;
	fstdout->mode = 'E' ;

  FICHIER *f1, *f2;
  int n;
  if (argc != 3) exit (-1);

  f1 = ouvrir (argv[1], 'E');
  if (f1 == NULL) exit (-1);

  n = fecriref (fstdout, "Test char: '%c' and string: '%s' with normal number 12\n", 'a', "bonjour");
  fecriref (fstdout, "Test number: '%d' char written on the above line\n", n);

  for (n=-11;n<11;n++)
    fecriref (fstdout, "%d ", n);
  fecriref (fstdout, "\n", n);
  
  vider(fstdout) ;

#if 0
  // Error, as expected: 
  fecriref (f1, "Num: '%e'\n"); 
#endif
  
  fermer (f1);

  {
    char *c = malloc(20) ;
    int  d;
    char my_word[10] ;
    
    f2 = ouvrir (argv[2], 'L');
    
    
    
    fliref (f2, "  test %s \n", my_word);
    
    
    
    printf("ttttttt  %s", my_word);
    
    //ecriref ("Read '%s' and '%c' from %s \n", my_word, &c, argv[2]);
    //fliref (f2, "end %d %s", &d, my_word);
    //ecriref ("Now read '%d' and '%s' from %s\n", d, my_word, argv[2]);

    fermer (f2);
    vider(fstdout) ;
  }
  
  return 0;
}

























