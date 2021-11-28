#include<stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdarg.h>
#include"stdes.h"

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
	int p_taille = taille*nbelem ;
	if(f->mode == 'L'){
		if(p_taille > f->taille + SIZE ){
			copier((f->tampon + f->place), (char*)p, f->taille) ;
			n = read(f->fd, (char*)p + f->taille, p_taille - f->taille );
			nb_o = n + f->taille ;
			f->taille = 0 ;
			f->place = 0 ;
		}
		else{
			if(p_taille  <= f->taille ){
				copier((f->tampon + f->place), (char*)p, p_taille) ;
				f->place += p_taille ;    
				f->place = f->place % SIZE ;
				f->taille -= p_taille ;   
				nb_o = p_taille ; 	
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
					f->taille += n ;
					nb_o = p_taille < f->taille ? p_taille : f->taille ;
					copier(f->tampon + f->place, (char*)p, nb_o) ;
					f->place += nb_o ;
					f->taille -= nb_o ;
				}
		  }
	  }
  }
	return nb_o/taille ;
}

int ecrire(const void *p, unsigned int taille, unsigned int nbelem, FICHIER *f){
	
	int n = 0 ;
	if(f == NULL )
	  return -1 ;
	
	int p_taille =  taille*nbelem ;
	if(f->mode == 'E'){
		if(p_taille > SIZE ){
			vider(f) ;
			n = write(f->fd, p, p_taille) ;
		}
		else{
			if(p_taille <= f->taille ){
				copier( (char*)p, f->tampon + f->place, p_taille) ;
				f->taille -= p_taille ; 
				f->place += p_taille ;	 
			}
			else{
				vider(f) ;
				copier( (char*)p, f->tampon + f->place, p_taille) ;
				f->taille -= p_taille ;
				f->place += p_taille ;
			}
			n = p_taille ;
		}
		
	}
	
	return n/taille ;
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

int fecriref (FICHIER *f, const char *format, ...){

	va_list ap;
	char *fr = format ;
	int d;
	char c, *s;
  int n = 0 ;
  
	va_start(ap, format);
	while(*fr){
		if(*fr == '%'){
			*fr++ ;
			switch (*fr) {
		        case 's':              /* string */
		           s = va_arg(ap, char *);
		           ecrire(s, 1, strlen(s), f);
		           n += strlen(s);
		           break;
		        case 'd':              /* int */
		           d = (int) va_arg(ap, int);
		           char *g = conv_int_to_char(d, g) ;
		           ecrire(g, 1, strlen(g), f);
		           free(g) ;
		           n += strlen(g);
		           break;
		        case 'c':              /* char */
		           c = va_arg(ap, int);
		           ecrire(&c, 1, 1, f);
		           n++ ;
		           break;
		  }
		        *fr++ ;
     }
   	 else{	
   	 	ecrire(fr, 1, 1, f);
   	 	*fr++ ;
   	 	n++ ;
   	 }
  }

        va_end(ap); 
	return n;
}

/* directly in stdout */
int ecriref (const char *format, ...){
	va_list ap;
	char *fr = format ;
	int d;
	char c, *s;
  int n = 0 ;
	va_start(ap, format);
	while(*fr){
		if(*fr == '%'){
			*fr++ ;
			switch (*fr) {
		        case 's':              /* string */
		           s = va_arg(ap, char *);
		           ecrire(s, 1, strlen(s), fstdout);
		           n += strlen(s) ;
		           break;
		        case 'd':              /* int */
		           d = (int) va_arg(ap, int);
		           char *g = conv_int_to_char(d, g) ;
		           ecrire(g, 1, strlen(g), fstdout);
		           free(g) ;
		           n += strlen(g) ;
		           break;
		        case 'c':              /* char */
		           c = va_arg(ap, int);
		           ecrire(&c, 1, 1, fstdout);
		           n++ ;
		           break;
		        }
		        *fr++ ;
           	 }
           	 else{	
           	 	ecrire(fr, 1, 1, fstdout);
           	 	*fr++ ;
           	 	n++;
           	 }
        }
        
        va_end(ap); 
	return n;
}

int fliref (FICHIER *f, const char *format, ...){
	va_list ap;
	char *fr = format ;
	int *d;
	char *c, *s, g[20], *pt, tmp;
  int n = 0 ;
	va_start(ap, format);
	while(*fr != '\0'){
		if(*fr == '%'){
			*fr++ ;
			switch (*fr) {
        case 's':              /* string */
           s = va_arg(ap, char *);
           pt = s;
           
           lire(pt, 1, 1, f);
           
           while(est_blanc(*pt) ){
           	lire(pt, 1, 1, f);
           }
           
           while(!est_blanc(*pt) ){
           	pt++;
           	lire(pt, 1, 1, f);
           }
           
           *pt = '\0' ;
           n++ ;
           break;
        case 'd':              /* int */
           pt = g ;
           
           d = va_arg(ap, int*);
           
           lire(pt, 1, 1, f);
           while(*pt < 47 || *pt >58){ 
            tmp = *pt ;
           	lire(pt, 1, 1, f);
           }
           while(*pt > 47 && *pt < 58){
            pt++;
           	lire(pt, 1, 1, f);
           }
           *pt = '\0' ;
           *d = conv_char_to_int(g) ;
           if(tmp == '-' )
            *d *= -1 ;
           n++ ;
           break;
        case 'c':              /* char */
           c = va_arg(ap, char *);
           lire(c, 1, 1, f);
           n++;
           break;
      }
    }
    *fr++;
  }
        
  va_end(ap); 

	return n;
}
// decaler les elements du tampon contenu dans f 
void decaler(FICHIER *f){
	copier((f->tampon + f->place), f->tampon, f->taille) ;
	f->place = 0 ;
}

// convertir un int to char
char* conv_int_to_char(int int_, char *s ){
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

// convertir un char en un int
int conv_char_to_int(char *s){
  int n = strlen(s) ;
  int p = 1 ;
  int res = 0;
  
  for(int i = n-1; i >= 0; i--){
    res += (s[i]-48)*p ;
    p *= 10;
  }
  
  return res ;
}

//copier nb_element de T1 dans T2
void copier(char *T1, char *T2, int nb_element){
	if( T1 != NULL && T1 != NULL){ 
		for(int i = 0; i < nb_element; i++){
			T2[i] = T1[i] ; 	
		}
	}
}

// on test si c est un char blanc
int est_blanc( char c ){
  if(c == ' ' ||  c == '\t' || c == '\n' || c == '\v' || c =='\f' || c == '\r' )
    return 1 ;

  return 0 ;
}

























