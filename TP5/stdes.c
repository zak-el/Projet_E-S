#include<stdlib.h>
#include<stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdarg.h>
#include"stdes.h"

//#include <stdio.h>


FICHIER *ouvrir(const char *nom, char mode){
	int fd;
	FICHIER *f = malloc(sizeof(FICHIER));
	if(f != NULL){
	
		if(mode == 'L'){
			fd = open(nom,O_RDONLY, 0644) ;
			f->taille = 0 ;
		}
		else if(mode == 'E'){
			fd = open(nom, O_WRONLY | O_CREAT, 0644) ;
			f->taille = SIZE ;
		}
		
		if(fd == -1){
			free(f);
			return NULL ;
		}
		
		f->fd = fd;
		f->mode = mode ;
		f->place = 0 ;
		
	}
	return f ;
}

int fermer(FICHIER*f){
	int res = -1;
	if(f != NULL ){
		if(f->mode == 'E')
			vider(f);
		res = close(f->fd);
		free(f);
	}
	return res;
}

int lire(void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
	int n ;
	int nb_o = 0; // nombre d'octets effictivement lus
	if(f->mode == 'L'){
		if(taille*nbelem > f->taille + SIZE ){
			
			copier((f->tampon + f->place), (char*)p, f->taille) ;
			n = read(f->fd, (char*)p + f->taille, taille*nbelem - f->taille );
			nb_o = n + f->taille ;
			f->taille = read(f->fd, f->tampon, SIZE);
			f->place = 0 ;
			
		}
		else{
			if(taille*nbelem  <= f->taille ){
				copier((f->tampon + f->place), (char*)p, taille*nbelem) ;
				f->place += taille*nbelem ;    
				f->place = f->place % SIZE ;
				f->taille -= taille*nbelem ;   
				nb_o = taille*nbelem ; 	
			}
			else{
			
				if(f->taille != 0) 
					decaler(f);
				if( (n = read(f->fd, f->tampon + f->taille, SIZE - f->taille) ) == 0){
					copier(f->tampon + f->place, (char*)p, f->taille) ;
					nb_o = f->taille ;
					f->place = 0 ;
					f->taille = 0 ;
				}
				else{
					
					nb_o = taille*nbelem<n ? taille*nbelem : n ;
					copier(f->tampon + f->place, (char*)p, nb_o) ;
					f->place += nb_o ;
					f->taille = n - nb_o ;
				}
					
					
				
			}
		}
	}
	return nb_o/taille ;
}

int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
	
	//int nb_o = 0 ;
	int n ;
	int p_taille =  taille*nbelem ;
	if(f->mode == 'E'){
		if(p_taille > SIZE ){
		
			vider(f) ;
			n = write(f->fd, p, p_taille) ;
			f->taille = SIZE ;
			f->place = 0 ;
		
		
		}
		else{
			if(p_taille <= f->taille ){
				copier( (char*)p, f->tampon + f->place, p_taille) ;
				f->taille -= p_taille ; //f->taile = 0
				f->place += p_taille ;	 //f->place = 499
			}
			else{
				vider(f) ;
				copier( (char*)p, f->tampon + f->place, p_taille) ;
				f->taille -= p_taille ;
				f->place += p_taille ;
			}
		}
		
	}
	
	return nbelem ;
}

int vider(FICHIER *f){
	int n = 0 ;
	if(f->mode == 'E' && f->taille != SIZE){
		n = write(f->fd, f->tampon, SIZE - f->taille)  ;
		f->taille = SIZE ;
		f->place = 0 ;
	}
	
	return n;
}

void decaler(FICHIER *f){
	copier((f->tampon + f->place), f->tampon, f->taille - f->place) ;
	f->place = 0 ;
}

int fecriref (FICHIER *f, const char *format, ...){

	va_list ap;
	char *fr = format ;
	//char *fr = &frINT ;
	int d;
	char c, *s;

	va_start(ap, format);
	while(*fr){
		if(*fr == '%'){
			*fr++ ;
			switch (*fr) {
		        case 's':              /* string */
		           s = va_arg(ap, char *);
		           ecrire(s, 1, strlen(s), f);
		           //printf("%s", s);
		           break;
		        case 'd':              /* int */
		           d = (int) va_arg(ap, int);
		           char *g = convertir(d, g) ;
		           ecrire(g, 1, strlen(g), f);
		           
		           //printf("inttttttttttttt %d", d);
		           break;
		        case 'c':              /* char */
		           c = va_arg(ap, int);
		           ecrire(&c, 1, 1, f);
		           //printf("char %c", c);
		           break;
		        }
		        *fr++ ;
           	 }
           	 else{	
           	 	ecrire(fr, 1, 1, f);
           	 	*fr++ ;
           	 }
        }
        
        va_end(ap); 
	return 0;
}

char* convertir(int int_, char *s ){
	s = malloc(20) ;
	char *t = malloc(20) ;
	char *debut ;
	int d ;
	int i = 0 ;
	
	if(int_ < 0 ){
		d = -int_ ;
		debut = t + 1 ;
		t[0] = '-' ;
	}
	else if( int_ ==0 ){
		t[0] = 48 ;
		t[1] = '\0' ;
		return s = t ;
	}
	else{
		d = int_ ;
		debut = t ;
	}
	
	while( d != 0 && i < 19 ){
		s[i] = d%10 + 48 ;
		d /= 10 ;
		i++;
	}
	s[i] = '\0';
	t[i] = '\0' ;
	int n = i-1 ;
	
	for(i = n; i>=0; i--){
		debut[n-i] = s[i] ;
	}
	
	return s = t ;
}

/* directly in stdout */
int ecriref (const char *format, ...){
	va_list ap;
	char *fr = format ;
	//char *fr = &frINT ;
	int d;
	char c, *s;

	va_start(ap, format);
	while(*fr != '\0'){
		if(*fr == '%'){
			*fr++ ;
			switch (*fr) {
		        case 's':              /* string */
		           s = va_arg(ap, char *);
		           ecrire(s, 1, strlen(s), fstdout);
		           //printf("%s", s);
		           break;
		        case 'd':              /* int */
		           d = (int) va_arg(ap, int);
		           char *g = convertir(d, g) ;
		           ecrire(g, 1, strlen(g), fstdout);
		           
		           //printf("inttttttttttttt %d", d);
		           break;
		        case 'c':              /* char */
		           c = va_arg(ap, int);
		           ecrire(&c, 1, 1, fstdout);
		           //printf("char %c", c);
		           break;
		        }
		        *fr++ ;
           	 }
           	 else
           	 	ecrire(fr, 1, 1, fstdout);
           	 	*fr++;
        }
        
        va_end(ap); 

	return 0;
}

int fliref (FICHIER *f, const char *format, ...){
	va_list ap;
	char *fr = format ;
	//char *fr = &frINT ;
	int *d;
	char *c, *s;

	va_start(ap, format);
	while(*fr != '\0'){
		if(*fr == '%'){
			*fr++ ;
			switch (*fr) {
		        case 's':              /* string */
		           s = va_arg(ap, char *);
		           char pt ;
		           
		           lire(&pt, 1, 1, f);
		           if(pt == ' ' || pt == '\t' || pt == '\n' ){
		           	lire(&pt, 1, 1, f);
		           }
		           while(pt != ' ' && pt != '\t' && pt != '\n' ){
		           	
		           	lire(&pt, 1, 1, f);
		           	printf("|%c|", pt) ;
		           	//printf("llllllllll %c llllllll", *pt) ;
		           }
		           	
		           //*pt = '\0' ;
		           
		           break;
		        case 'd':              /* int */
		           d = va_arg(ap, int*);
		           char *g = convertir(d, g) ;
		           lire(g, 1, strlen(g), f);
		           
		           break;
		        case 'c':              /* char */
		           c = va_arg(ap, char *);
		           lire(c, 1, 1, f);
		           break;
		        }
           	 }
           	 *fr++;
        }
        
        va_end(ap); 

	return 0;
}

//copier nb_element de T1 dans T2
void copier(char *T1, char *T2, int nb_element){
	if( T1 != NULL && T1 != NULL){ 
		for(int i = 0; i < nb_element; i++){
			T2[i] = T1[i] ; 	
		}
	}
}



























