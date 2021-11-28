#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include"stdes.h"

#define MEMORY_SIZE 12800
#define REPEAT 10

char MEMORY[MEMORY_SIZE];


void init_mem(void)
{
  int i;
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_usec);

  for (i=0; i<MEMORY_SIZE; )
    MEMORY[i++] = (char) (rand () % 95 + 0x20); /* printable characters */
}

int main(int argc, char *argv[]){
  
  
  init_mem() ;
  char *nom = "file_name" ;
  char c ;
  clock_t start, end ;
  double elapsed1 = 0, elapsed2 = 0 ;
  
  for(int j=0; j<REPEAT; j++){
  
    start = clock();
    
    int fd = open(nom, O_WRONLY | O_CREAT, 0644) ;
    
    for(int i =0; i<MEMORY_SIZE; i++){
      write(fd,&MEMORY[i],1) ;
    }
    
    close(fd) ;
    fd = open(nom, O_RDONLY, 0644) ;
    
    for(int i =0; i<MEMORY_SIZE; i++){
      read(fd,&c,1) ;
      //write(1,&c,1) ;
    }
    close(fd) ;

    end = clock();
    elapsed1 += (double)(end - start)/CLOCKS_PER_SEC;
    
    
    /*******************************************/
    
    start = clock();
    
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

    end = clock();
    elapsed2 += (double)(end - start)/CLOCKS_PER_SEC;
    
    
    
  }
  
  printf("Time measured using system calls: %.3f seconds.\n", elapsed1/REPEAT);
  printf("Time measured using our functions: %.3f seconds.\n", elapsed2/REPEAT);

  return 0 ;
}

