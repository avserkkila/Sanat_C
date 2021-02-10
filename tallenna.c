#include "aliohjelmat.h"

extern char **auki_oleva;

/*Palauttaa montako komentoa tallentamiseen liitettiin: n = {0,1,2}*/
int tallenna(snlista *l, strlista* s) {
  char *kohde = calloc(MAXLEN_STDIN, 1);
  if (!kohde) return -1;
  
  FILE *f;
  char nargin = 2;
  if ( STR_END(s->str) == ';' || !(s = s->seur) ) { // vain yksi argumentti
    strcpy(kohde, *auki_oleva);
    nargin = 0;
  }
  else if ( STR_END(s->str) == ';' ) { //kaksi argumenttia
    strcpy(kohde, s->str);
    STR_END(kohde) = '\0';
    nargin = 1;
  }
  
  /*Tarvittaessa lisää rivinvaihto loppuun*/
  if( !(f = fopen(kohde, "a+")) )
    return -1;
  fseek(f,-1,SEEK_END);
  if (fgetc(f) != '\n')
    fputc('\n',f);

  /*tallentaa loppuun alkaen annetusta jäsenestä*/
  do {
    fprintf(f,"%s\n",l->sana);
    fprintf(f,"%s\n",l->kaannos);
    l = l->seur;
  } while(l->parin_nro != -1);
  
  fclose(f);
  if( !(s = s->seur) || nargin != 2 )
    return nargin;

  /*Siirretään tiedosto toisen nimiseksi*/
  if (STR_END(s->str) == ';')
    STR_END(s->str) = '\0';
  char *apu;
  if( !(apu = calloc(MAXLEN_STDIN,1)) )
    return -1;
  strcat(strcat(strcpy(apu, "mv -i "), kohde), s->str);
  system(apu);
  
  free(kohde);
  free(apu);
  return nargin;
}
