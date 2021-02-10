#include <ctype.h>
#include "aliohjelmat.h"

char* bitit(char c) {
  char *r = calloc(8,1);
  unsigned char yksi = 1;
  yksi = yksi << 7;
  for (int i=0; i<8; i++)
    r[i] = (yksi & (c << i))? 1 : 0;
  return r;
}

/*10-alkuiset ovat jatkoa aiemmin alkaneelle merkille
  niitä ei siis lasketa ja muut lasketaan*/
int nmerkit(char* str) {
  int n = 0;
  char *bitti;
  for (int i=0; i<strlen(str); i++) {
    bitti = bitit(str[i]);
    if ( !bitti[0] || (bitti[0] && bitti[1]) )
      n++;
    free(bitti);
  }
  return n;
}

strlista* pilko_sanoiksi(char* s) {
  strlista* l = NULL;
  FILE *stream = fmemopen(s, strlen(s), "r");
  if(!stream) return NULL;
  while(!feof(stream)) {
    char *sana = calloc(strlen(s)+1,1);
    if(!sana) return NULL;
    fscanf(stream, "%s", sana);
    sana = realloc(sana, strlen(sana)+1);
    if (!(l = _ylisaa(l, sana))) return NULL;
  }
  fclose(stream);
  return _yalkuun(l);
}

char str_is_num(char *s) {
  do
    if(*s < '0' || *s > '9')
      return 0;
  while(*(++s));
  return 1;
}
