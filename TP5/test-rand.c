#include "stdes.h"
#include <stdlib.h>
#include <sys/time.h>
#include <assert.h>

#define MEMORY_SIZE 1280000 /* 1,3 Mo */

/*#define DEBUG*/

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

#define RAND_NUM 1 + rand () % 128 + 100 * (rand () % 3) + (4000 * ((rand () % 4)/4) * (rand() & 1))

void mem_write (FICHIER* f)
{
  int count = 0;
  int num;
  int lr;
  ecriref ("Writing file...\n");

  while (count < MEMORY_SIZE) {
    num = RAND_NUM ;

    if (count + num > MEMORY_SIZE)
      num = MEMORY_SIZE - count;

#ifdef DEBUG
    ecriref ("Writting % 8d / % 8d octets\n", num, count);
#endif

    lr = ecrire (MEMORY + count, 1, num, f);
#ifdef DEBUG
    if (lr != num)
      ecriref (" -- Wrote only %d octets\n", lr);
#endif
    count += lr;
  }
  ecriref ("Done\n");
  vider (fstdout);
}

void mem_read (FICHIER* f, char* buff)
{
  int count = 0;
  int num;
  int lr;
  ecriref ("Reading file...\n");
  
  fstdout = (FICHIER*)malloc(sizeof(FICHIER));
	fstdout->fd = 1 ;
	fstdout->place = 0 ;
	fstdout->taille = SIZE ;
	fstdout->mode = 'E' ;
	
	fstderr = (FICHIER*)malloc(sizeof(FICHIER));
	fstderr->fd = 2 ;
	fstderr->place = 0 ;
	fstderr->taille = SIZE ;
	fstderr->mode = 'E' ;
  
  
  
  do {
    num = RAND_NUM ;
#ifdef DEBUG
    ecriref ("Reading  % 8d \\ % 8d octets\n", num, count);
#endif
    lr = lire (buff, 1, num, f);
#ifdef DEBUG
    if (lr != num)
      ecriref (" -- Read only %d octets\n", lr);
#endif
    buff += lr;
    count += lr;
    assert (count <= MEMORY_SIZE);
  } while (lr);
  ecriref ("Done\n");
  vider (fstdout);
}

void mem_compare (char* ref, char *buff)
{
  int i;
  ecriref ("Comparing memories...\n");
  for (i=0; i<MEMORY_SIZE; i++) {
    if (ref[i] != buff[i]) {
      fecriref(fstderr, "ERROR %c != %c at index %d\n", 
          ref[i], buff[i], i);
    }
  }
  ecriref ("Done\n");
  vider (fstdout);
}


int main(int argc, char *argv[])
{
  FICHIER *f;
  char* filename = "rand-file.txt";

  init_mem ();

  f = ouvrir (filename, 'E');
  if (f == NULL)
    exit (-1);
  mem_write (f);
  fermer (f);

  {
    char *buffer = malloc (sizeof(char) * MEMORY_SIZE);
    f = ouvrir (filename, 'L');
    if (f == NULL)
      exit (-1);
    mem_read (f, buffer);
    mem_compare (MEMORY, buffer);
    free (buffer);
    fermer (f);
  }

  return 0;
}
