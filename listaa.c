#include <dirent.h>
#include "aliohjelmat.h"

char listaa_str_vrt(char* vertaaja, char* verrattava);

strlista* listaa(strlista** knnot_p) {
  strlista* knnot = *knnot_p;
  char *list_arg = malloc(1); //argumentti, jonka mukaan valitaan, mitä otetaan listalle
  DIR *d = opendir("./");
  if(!d)
    return NULL;
  struct dirent *e;
  if(!d) {
    fprintf(stderr, "Kansiota ei voitu avata\n");
    return NULL;
  }
  strlista* lis = NULL;
  char *nimi;
  while ((e = readdir(d))) {
    nimi = calloc(strlen(e->d_name)+1,1);
    strcpy(nimi, e->d_name);
    lis = _ylisaa(lis, nimi);
  }
  closedir(d);

  lis = _yalkuun(lis);
  strlista *toimiva = lis;
  
  /* nargin == 1*/
  if ( !(knnot->seur) || !strcmp(knnot->str, "LISTAA;") )
    {
      /*poistetaan tyhjät, ., .., ja *~ */
      /*Lopussa lis on NULL, jota ei voida palauttaa,
	joten otetaan aina edellinen arvo talteen*/
      while(lis) {
	if ( STR_END(lis->str) == '~' || !strcmp(lis->str, ".")	\
	     || !strcmp(lis->str, "..") || !strcmp(lis->str, "") )  //nämä halutaan poistaa listalta
	  {
	    if (lis->edel)
	      {
		lis = lis->edel;
		_ypoista1(lis->seur, _strvapautus, 1);
		toimiva = lis;
		lis = lis->seur;
	      }
	    else if(lis->seur) // Tämä ei voi olla silmukan viimeinen
	      {
		lis = lis->seur;
		_ypoista1(lis->edel, _strvapautus, 1);
	      }
	    else
	      {
		toimiva = NULL;
		_ypoista1(lis, _strvapautus, 1);
	      }
	  }
	else
	  {
	    toimiva = lis;
	    lis = lis->seur;
	  }
      }
    }
  else

    /* nargin > 1 */
    {
      strlista* uusi  = NULL;
      char katkaise = 0; //Jos nykyinen päättyy ';'-merkkiin, katkaise silmukka seuraavalla

      /*Listataan vain yhden ehdon mukaan kussakin silmukassa*/
      while ( !katkaise && knnot->seur ) {
	lis = toimiva;  //edellisellä kierroksella osoitin on viety nulliin asti
	knnot = _ypoista1(knnot, _strvapautus, 1);
	if ( !(list_arg = realloc(list_arg, strlen(knnot->str)+1)) ) {
	  lis = NULL;
	  goto LISTAAMISEN_LOPPU;
	}
	strcpy(list_arg, knnot->str);
    
	/*Tarvittaessa poistetaan ';' lopusta ja asetetaan katkaisu*/
	if(STR_END(knnot->str) == ';') {
	  katkaise = 1;
	  if (STR_END(list_arg) == ';')
	    STR_END(list_arg) = '\0';
	}

	/*Tehdään uusi lista niistä, jotka täyttävät ehdon*/
	do
	  if (!listaa_str_vrt(list_arg, lis->str)) {
	    char *tmp = calloc(strlen(lis->str)+1, 1);
	    strcpy(tmp, lis->str);
	    uusi = _ylisaa(uusi, tmp);
	  }
	while ((lis=lis->seur));
      }
      _ypoista_kaikki(toimiva, _strvapautus); //poistetaan käsittelemätön lista
      toimiva = uusi;
    }

  lis = _yalkuun(toimiva);
  _strjarjesta(lis);
  _strtulosta(lis);
 LISTAAMISEN_LOPPU:
  *knnot_p = knnot;
  return lis;
}

char listaa_str_vrt(char* vertaaja_suote, char* verrattava_suote) {
  char r;
  char* vertaaja   = malloc(strlen(vertaaja_suote  ) +1 );
  char* verrattava = malloc(strlen(verrattava_suote) +1 );
  strcpy(vertaaja, vertaaja_suote);
  strcpy(verrattava, verrattava_suote);
  
  /*haetaan luvun paikka ja katkaistaan molemmat siitä*/
  int paikka;
  for(paikka=0; paikka<strlen(vertaaja_suote); paikka++)
    if (vertaaja[paikka] == '(' && vertaaja[paikka+1] == ')') {
      vertaaja[paikka] = '\0';
      if ( paikka<strlen(verrattava) )
	verrattava[paikka] = '\0';
      break;
    }
  
  /*verrataan lukuja tähän asti*/
  if (strcmp(vertaaja, verrattava)) {
    r = 1;
    goto LOPPU;
  }
  
  /*Haetaan loppuosa poistamalla sulut vertaajasta ja mahdollinen luku verrattavasta.
    Sallitaan siis myös se, että lukua ei ole*/
  strcpy(vertaaja,   vertaaja_suote+paikka+2);
  while('0' <= verrattava_suote[paikka] && verrattava_suote[paikka++] <= '9');
  strcpy(verrattava, verrattava_suote+paikka);

  /*Verrataan loppuosaa*/
  r = strcmp(vertaaja, verrattava);
  
 LOPPU:
  free(vertaaja);
  free(verrattava);
  return r;
}
