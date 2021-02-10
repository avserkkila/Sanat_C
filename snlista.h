#ifndef _snlista_h_
typedef struct snlista_s {
  struct snlista_s* edel;
  char* sana;
  struct snlista_s* seur;
  char* kaannos;
  int   parin_nro;
  int   tiedostonro;
} snlista;
#endif
#define _snlista_h_

snlista* snuusi_lista(void);
snlista* snlisaa(snlista*);
snlista* snjatka(snlista*);
snlista* snkopioi(snlista*);
snlista* snpoista1(snlista*);
void     snpoista(snlista*, int* nrot, int kpl);
void     snpuorauta(snlista*);
void     snvapautus(void*);
snlista* sekoita(snlista*);

snlista* snnouda(snlista*, int);
