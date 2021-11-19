#ifndef _STDES_H
#define _STDES_H

#define SIZE 500

struct _ES_FICHIER{
	int fd; 	// descripteur de fichier
	char taumpon[SIZE];
	int place; 	//debut du taumpon
	int taille; 	// nombre de l'elements dans le taumpon
	char mode;	//read ("L") / wrie ("E")
};

typedef struct _ES_FICHIER FICHIER;

FICHIER *stdout;
FICHIER *stderr;

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

#endif

