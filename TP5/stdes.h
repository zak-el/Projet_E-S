#ifndef _STDES_H
#define _STDES_H

#define SIZE 10

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
FICHIER *ouvrir(const char *nom, char mode);
int fermer(FICHIER*f);
int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
int vider(FICHIER *f);

int fecriref (FICHIER *f, const char *format, ...);
/* directly in stdout */
int ecriref (const char *format, ...);
int fliref (FICHIER *f, const char *format, ...);

void copier(char *T1, char *T2, int nb_element);
void decaler(FICHIER *f);
char* conv_int_to_char(int int_, char *s );

#endif

