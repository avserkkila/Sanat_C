#include <time.h>
#include <math.h>
#include "aliohjelmat.h"
#include "ilista.h"

snlista* snuusi_lista(void) {
  snlista* l = malloc(sizeof(snlista));
  if(!l) return NULL;
  l->edel        = NULL;
  l->sana        = NULL;
  l->kaannos     = NULL;
  l->parin_nro   = 0;
  l->tiedostonro = 0;
  l->seur        = NULL;
  return l;
}

snlista* snjatka(snlista* l) {
  snlista* uusi = snuusi_lista();
  if(!uusi) return NULL;
  return _ylsvlms(l, uusi);
}

snlista* snkopioi(snlista* l) {
  snlista* uusi = NULL;
  while(l) {
    if(!(uusi = snjatka(uusi))) return NULL;
    uusi->sana = malloc(strlen(l->sana)+1);
    uusi->kaannos = malloc(strlen(l->kaannos)+1);
    if(!(uusi->sana) || !(uusi->kaannos)) return NULL;
    strcpy(uusi->sana,    l->sana);
    strcpy(uusi->kaannos, l->kaannos);
    uusi->parin_nro = l->parin_nro;
    uusi->tiedostonro = l->tiedostonro;
    l = l->seur;
  }
  return _yalkuun(uusi);
}

void snpuorauta(snlista *l) {
  char *apu;
  while(l) {
    apu = l->sana;
    l->sana = l->kaannos;
    l->kaannos = apu;
    l = l->seur;
  }
}

void snvapautus(void* lv) {
  snlista* l = lv;
  free(l->sana);
  free(l->kaannos);
}

snlista* snnouda(snlista* l, int i) {
  return _ynouda(l, i);
}

snlista* sekoita(snlista* l) {
  /*Järjestyksen arpomismenetelmä:
    1. tehdään lista luvuista 0 – listan pituus
    2. arvotaan luku n kyseiseltä väliltä
    3. otetaan listalta n. luku,
       sijoitetaan se uuteen taulukkoon ja poistetaan listalta
    4. toistetaan arpomalla aina luku väliltä 0 – listan pituus*/
  ilista* il = NULL;
  ilista* apuil;
  int ularaja = _ylaske(l);
  snlista* jarj[ularaja];
  
  for(int i=0; i<ularaja; i++)
    il = _ilisaa(il, i);

  il = _yalkuun(il);
  srand(time(NULL));
  int i = 0;

  /*tehdään taulukko, johon sekoitetaan jäsenet.*/
  while (ularaja--){
    double rd = (double)ularaja/RAND_MAX * rand();
    int r = round(rd);
    apuil = _ynouda(il, r);
    if (r == 0)
      il = il->seur;
    int sijainti = apuil->i;
    jarj[i++] = _ynouda(l, sijainti);
    _yrm1(apuil, 1);
  }
  ularaja = _ylaske(l)-1;

  /*Järjestetään lista linkittämällä se uudestaan*/
  jarj[0]->seur = jarj[1];
  jarj[0]->edel = NULL;
  for (i=1; i<ularaja; i++) {
    jarj[i]->edel = jarj[i-1];
    jarj[i]->seur = jarj[i+1];
  }
  jarj[ularaja]->edel = jarj[ularaja-1];
  jarj[ularaja]->seur = NULL;
  return _yalkuun(l);
}
