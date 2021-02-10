#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "lista.h"
#include "makroja.h"
#include "ilista.h"

lista* uusi_lista(void) {
  lista* l = malloc(sizeof(lista));
  l->LIST_PITUUS = 0;
  l->parin_nro = -1;
  l->seur = l;
  l->edel = l;
  return l;
}

/*Lisää j:n listalle ennen jäsentä l, eli lopppuun jos l on kansi*/
lista* lisaa_valmis(lista* l, lista* j) {
  l->edel->seur = j;
  j->edel = l->edel;
  l->edel = j;
  j->seur = l;
  (l->LIST_PITUUS)++;
  return j;
}

/*Lisää loppuun tai ennen l:ää, jos l ei ole kansi. Palauttaa lisätyn jäsenen*/ 
lista* lisaa(lista* l) {
  lista *j = malloc(sizeof(lista));
  return lisaa_valmis(l, j);
}

/*ensimmäinen elementti on 0, kansi on -1*/
lista* nouda(lista* l, int nro) {
  for (int i=-1; i<nro; i++)
    l = l->seur;
  return l;
}

lista* poista(lista*l, int* nrot, int kpl) {
  lista *poistot[kpl];
  TOISTA(kpl)
    poistot[tmp] = nouda(l, nrot[tmp]);
  TOISTA(kpl)
    poista1(poistot[tmp]);
  return l;
}

void poista1(lista* poisto) {
  poisto->edel->seur = poisto->seur;
  poisto->seur->edel = poisto->edel;
  lista *l = poisto;
  while (l->parin_nro != -1)
    l = l->seur;
  (l->LIST_PITUUS)--;
  free(poisto->sana);
  free(poisto->kaannos);
  free(poisto);
}

void puorauta(lista* l) {
  char *apu;
  lista* kansi = l;
  l = l->seur;
  while(l != kansi) {
    apu = l->sana;
    l->sana = l->kaannos;
    l->kaannos = apu;
    l = l->seur;
  }
}

void sekoita(lista* l) {
  /*Järjestyksen arpomismenetelmä:
    1. tehdään lista luvuista 0 – listan pituus
    2. arvotaan luku n kyseiseltä väliltä
    3. otetaan listalta n. luku,
       sijoitetaan se uuteen taulukkoon ja poistetaan listalta
    4. toistetaan arpomalla aina luku väliltä 0 – listan pituus*/
  ilista* il = NULL;
  ilista* apuil;
  int maara = l->LIST_PITUUS;
  lista* jarj[maara+2];
  
  for(int i=0; i<maara; i++)
    il = _ilisaa(il, i);

  il = _yalkuun(il);
  srand(time(NULL));
  int i = 0;

  /*tehdään taulukko, johon sekoitetaan jäsenet.
    Ympärilinkitystä varten aloitetaan vasta toisesta*/
  while (maara--){
    double rd = (double)maara/RAND_MAX * rand();
    int r = round(rd);
    apuil = _ynouda(il, r);
    int sijainti = apuil->i;
    jarj[++i] = nouda(l, sijainti);
    _yrm1(apuil);
  }

  maara = l->LIST_PITUUS;
  jarj[0]       = jarj[maara];
  jarj[maara+1] = jarj[1];

  /*Järjestetään lista linkittämällä se uudestaan*/
  for (i=1; i<=maara; i++) {
    jarj[i]->edel = jarj[i-1];
    jarj[i]->seur = jarj[i+1];
  }
}

lista* kopioi(lista* l) {
  lista* uusi = uusi_lista();
  while((l=l->seur)->parin_nro != -1) {
    lista *jasen = lisaa(uusi);
    jasen->sana        = calloc(MAXLEN_STDIN,1);
    jasen->kaannos     = calloc(MAXLEN_STDIN,1);
    jasen->parin_nro   = l->parin_nro;
    jasen->tiedostonro = l->tiedostonro;
    strcpy(jasen->sana,    l->sana);
    strcpy(jasen->kaannos, l->kaannos);
  }
  return uusi;
}

/*Tulostaa kaikki jäsenet ja viimeisenä kansilehden*/
void tulosta(lista* l) {
  int i = 0;
  do {
    l = l->seur;
    printf("%i:\n",i++);
    printf("Sana:             %s\nKäännös:          %s\nParin numero:     %i\nTiedoston numero: %i\n\n", //
	   l->sana, l->kaannos, l->parin_nro, l->tiedostonro);
  } while(l->parin_nro != -1);
}
