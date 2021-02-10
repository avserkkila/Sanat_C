/*| Lista, jonka kansilehtenä on samanlainen |
  | elementti ja jonka on ympäri linkitetty  |
  |––––––––––––––––––––––––––––––––––––––––––|*/

#define LIST_PITUUS tiedostonro

#ifndef lista
typedef struct ls_jasen {
  char* sana;
  char* kaannos;
  int parin_nro;
  int tiedostonro;
  struct ls_jasen *edel;
  struct ls_jasen *seur;
} lista;
#endif

lista* uusi_lista(void);
/*Tämä palauttaa lisätyn jäsenen*/
lista* lisaa(lista* l);
lista* lisaa_valmis(lista* l, lista* lisays);
lista* nouda(lista* l, int nro);
lista* poista(lista* l, int* nrot, int kpl);
void   poista1(lista* poisto);
void   sekoita(lista*);
lista* kopioi(lista* l);
void   puorauta(lista* l);
void   tulosta(lista* l);
