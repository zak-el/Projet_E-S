#include<stdlib.h>
#include<stdio.h>
#include<stdes.c>


FICHIER *ouvrir(const char *nom, char mode){
	int fd;
	FICHIER *f = malloc(sizeof(FICHIER));
	if(f != NULL){
		if(mode == 'L')
			fd = open(nom,O_RDONLY) ;
		else if(mode == 'E')	
			fd = open(nom,O_WRONLY) ;
			
		if(fd == -1){
			free(f);
			return NULL ;
		}
		f->fd = fd;
		f->mode = mode	
		f->debut = 0 ;
		f->fin = 0 ;
	}
	return f ;
}

int fermer(FICHIER*f){
	int res = -1;
	if(f != NULL){
		vider(f);
		res = close(f->fd);
		free(f);
	}
	return res;
}

int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
	int n; // nombre d'octets effictivement lus
	if(f->mode == 'L'){
		if(taille*nbelem > SIZE ){
			vider(f);
			n = read(f->fd, p, taille*nbelem );
			f->taille = read(f->fd, f->taumpon, taille*nbelem );
			f->place = 0 ;
		}
		else{
			if((f->taille - f->place) =< taille*nbelem ){
				copier((char*)p, (f->taumpon + f->place)) ;
				f->place += taille*nbelem ;
			}
			else{
				vider(f);
				n = read(f->fd, f->taumpon + f->place, taille*nbelem );
				f->taille += n ;
			}
		}
	}
	return n/nbelem ;
}

int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f);
int vider(FICHIER *f);

int fecriref (FICHIER *f, const char *format, ...);
/* directly in stdout */
int ecriref (const char *format, ...);
int fliref (FICHIER *f, const char *format, ...);

void copier(char *T1, char *T2){

}



























