#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "makroja.h"
#include "snlista.h"
#include "ylista.h"
#include "strlista.h"

#ifndef _krrs_tdt_
typedef struct {
  int* osatut;
  int n_osattuja;
  snlista* l;
  char** suotteet;
} krrs_tdt;
#endif
#define _krrs_tdt_

snlista* avaa(char* nimi);
krrs_tdt kierros(krrs_tdt k);
krrs_tdt alusta_kierros(snlista *l);
int tallenna(snlista*, strlista* knnot);
strlista* listaa(strlista** knnot);
char korjaa(snlista *l, strlista** args);
