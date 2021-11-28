#ifndef _STDES_H
#define _STDES_H

#define SIZE 100000

struct _ES_FICHIER{
	int fd; 	// descripteur de fichier
	char tampon[SIZE];
	int place; 	//debut du taumpon
	int taille; 	// nombre d'octets disponibles dans le tampon
	char mode;	//read ("L") / wrie ("E")
};

typedef struct _ES_FICHIER FICHIER;

FICHIER *fstdout;
FICHIER *fstderr;

/* mode: 'L' = lecture, 'E' = écriture */
FICHIER *ouvrir(const char *, char );
int fermer(FICHIER*);
int lire(void *, unsigned int , unsigned int , FICHIER *);
int ecrire(const void *, unsigned int , unsigned int , FICHIER *);
int vider(FICHIER *);

int fecriref (FICHIER *, const char *, ...);
/* directly in stdout */
int ecriref (const char *, ...);
int fliref (FICHIER *, const char *, ...);

void copier(char *, char *, int );
void decaler(FICHIER *);
char* conv_int_to_char(int , char * );
int conv_char_to_int(char *);
int est_blanc( char ) ;

#endif

