#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include"stdes.h"


char *MEMORY;


void init_mem(int MEMORY_SIZE)
{
  int i;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);

  MEMORY = malloc(MEMORY_SIZE) ;

  for (i=0; i<MEMORY_SIZE; )
    MEMORY[i++] = (char) (rand () % 95 + 0x20); /* printable characters */
}


int main(int argc, char *argv[]){

  int MEMORY_SIZE = atoi(argv[1]) ;
  
  init_mem(MEMORY_SIZE) ;
  
  char *nom = "file_name" ;
  clock_t start = clock();
  char c;
    
  FICHIER *f = ouvrir(nom, 'E') ;
  
  for(int i =0; i<MEMORY_SIZE; i++){
    ecrire(&MEMORY[i],1,1,f) ;
  }
  
  fermer(f) ;
  
  f = ouvrir(nom, 'L') ;
  for(int i =0; i<MEMORY_SIZE; i++){
    lire(&c,1,1,f) ;
  }
  fermer(f) ;

  clock_t end = clock();
  double elapsed = (double)(end - start)/CLOCKS_PER_SEC;

  printf("Time measured using system calls: %.3f seconds.\n", elapsed);


return 0 ;
}
