#include "aliohjelmat.h"
#include "omastring.h"
#include <dirent.h>

extern strlista* auki_oleva;

snlista* avaa(char* nimi) {
  FILE *f;
  if ( !(f  = fopen(nimi, "r")) )
    return NULL;
  snlista *l = NULL;
  int parin_nro = 0;
  
  while (!feof(f)) {
    //rivinvaihto lopussa ja tyhjät rivit välissä ohitetaan
    if (fgetc(f) == '\n' || feof(f))
      continue;
    fseek(f,-1,SEEK_CUR);
    l = snjatka(l);
    l->sana    = calloc(MAXLEN_STDIN,sizeof(char));
    l->kaannos = calloc(MAXLEN_STDIN,sizeof(char));
    if(!(l->sana) || !(l->kaannos)) return NULL;
    fgets(l->sana,   MAXLEN_STDIN,f);
    fgets(l->kaannos,MAXLEN_STDIN,f);
    
    /*poistetaan rivinvaihto lopusta*/
      if (l->sana[strlen(l->sana)-1] == '\n')
	l->sana[strlen(l->sana)-1] = 0;
      if (l->kaannos[strlen(l->kaannos)-1] == '\n')
	l->kaannos[strlen(l->kaannos)-1] = 0;

      /*Tiedostonumeron määrittämistä ei ole vielä lisätty,
	joten se on aina 0*/
      l->parin_nro = parin_nro++;
      l->tiedostonro = 0;
  }
  fclose(f);
  return _yalkuun(l);
}

#define VIRHE (krrs_tdt) {NULL}
krrs_tdt alusta_kierros(snlista *l) {
  krrs_tdt k;
  if ( !(k.osatut = malloc(sizeof(int)*(_ylaske(l)))) )
    return VIRHE;
  k.n_osattuja = 0;
  k.l = snkopioi(l);
  if ( !(k.suotteet = calloc(N_SUOTTEITA_KIERROKSELLA, sizeof(char*))) )
    return VIRHE;
  TOISTA( (N_SUOTTEITA_KIERROKSELLA) )
    if ( !(k.suotteet[tmp] = calloc(MAXLEN_STDIN, 1)) )
      return VIRHE;
  return k;
}
#undef VIRHE

char korjaa(snlista *l, strlista** args) {
  char r = 0;
  int missa_korjataan;
  int pareja_taakse;
  char tama_on_kaannos;
  char *korjaus;
  *args = _strpoista1(*args, 1);
  if (!str_is_num((*args)->str)) {
    printf("Anna korjattavan sijainti: ");
    if (!scanf("%i", &missa_korjataan))
      return 1;
  } else {
    sscanf((*args)->str, "%i", &missa_korjataan);
    *args = _strpoista1(*args, 1);
  }

  korjaus = (*args)->str;
  pareja_taakse = missa_korjataan/2;
  if(pareja_taakse*2 == missa_korjataan)
    tama_on_kaannos = 1;
  else
    tama_on_kaannos = 0;

  /*Korjataan lista*/
  for(int i=0; i<pareja_taakse; i++) {
    l = l->edel;
    if(!l) {
      printf("Ei pääse taaksepäin noin pitkästi. Lista loppui kohdassa %i/%i.\n",
	     i,pareja_taakse);
      return 1;
    }
  }
  char **korjattava_p = tama_on_kaannos? &(l->kaannos) : &(l->sana);
  *korjattava_p = realloc(*korjattava_p, strlen(korjaus)+1);
  strcpy(*korjattava_p, korjaus);

  /*Tiedoston korjaaminen:*/
  char* tiednimi = _strnouda((strlista*)_yalkuun(auki_oleva),l->tiedostonro)->str;
  FILE *f = fopen(tiednimi, "r+");
  if(!f) {
    fprintf(stderr, "Korjattavan tiedoston \"%s\" avaaminen ei onnistunut, ei korjattu\n", tiednimi);
    return r;
  }
  /*siirrytään oikealle riville, huomioidaan että voi olla tyhjiä rivejä välissä*/
  int parin_sij = l->parin_nro*2;
  for(int i=0; i<parin_sij; i++) {
    while(!feof(f) && fgetc(f) != '\n');
    while(!feof(f) && fgetc(f) == '\n');
  }
  if(tama_on_kaannos)
    while(!feof(f) && fgetc(f) != '\n');
    while(!feof(f) && fgetc(f) == '\n');
  if(feof(f)) {
    fprintf(stderr, "Virhe: EOF korjattavassa tiedostossa \"%s\", ei korjattu\n", tiednimi);
    fclose(f);
    return r;
  }
  fseek(f, -1, SEEK_CUR);
  FILE* korjauspaikka = f; //tallennetaan tämä oikea sijainti

  /*siirrytään rivin ohi ja kopioidaan loppu*/
  while(!feof(f) && fgetc(f) != '\n');
  char c;
  int koon_palikka = 200;
  long jalkiosan_koko = koon_palikka;
  char* jalkiosa = malloc(jalkiosan_koko);
  if(!jalkiosa) {
    fclose(f);
    return 1;
  }
  int i = 0;
  while(1) {
    for(; i<jalkiosan_koko-1; i++) {
      c = fgetc(f);
      if(c == EOF)
        goto POIS_SILMUKOISTA;
      jalkiosa[i] = c;
    }
    char* hatavara = jalkiosa;
    jalkiosa = realloc(jalkiosa, jalkiosan_koko+=koon_palikka);
    if(!jalkiosa) {
      free(hatavara);
      fclose(f);
      return 1;
    }
  }
 POIS_SILMUKOISTA:
  jalkiosa[i] = '\0';
  f = korjauspaikka;
  /*korjataan*/
  for(int i=0; korjaus[i]!='\0'; i++)
    fputc(korjaus[i], f);
  fputc('\n', f);
  /*liitetään jälkiosa*/
  for(int i=0; jalkiosa[i]!='\0'; i++)
    fputc(jalkiosa[i], f);
  
  fclose(f);
  free(jalkiosa);
  return r;
}
