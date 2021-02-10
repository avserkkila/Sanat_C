#include "aliohjelmat.h"
#include "kierroskomennot.h"

kierroskmnt_t kierroskomennoksi(char *suote) {
  kierroskmnt_t r;
  FILE *f;

  if(strlen(suote) == 1)
    switch(*suote) {
    case '1': return YKSI;
    case '0': return EXIT;
    case '<': return L_ARROW;
    case '!': return OLI_VAARIN;
    case '#': return RISUAITA;
    }
  if(!strcmp(suote,"§"))
    return TAMA_OIKEIN;
  
  f = fopen(CSANAT_DIR"/kierroskomennot.h","r");
  if (!f)
    return EI_HEADERIA;
  char* yrite = calloc(H_MAX_SANA,1);
  if(!yrite) {
    fclose(f);
    return VIRHE;
  }

  /*haetaan kohta "enum kierroskmnt_e {" */
  while (!feof(f)) {
    while( fscanf(f, "%s", yrite) == 1 && strcmp(yrite,"enum") );
    if (fscanf(f, "%s", yrite) == 1 && !strcmp(yrite,"kierroskmnt_e"))
      if (fscanf(f, "%s", yrite) == 1 && !strcmp(yrite,"{"))
	goto LUE_KIERROS_ENUM;
  }
  free(yrite);
  return EI_ENUMIA_HEADERISSA;

  LUE_KIERROS_ENUM:
  /*haetaan annettua sanaa, lopussa voi olla pilkku */
  r = ENUM_ALKU;
  char* suote1 = calloc(H_MAX_SANA,1);
  strcpy(suote1,suote);
  strcat(suote1,",");
  while( fscanf(f, "%s", yrite) == 1 && strcmp(yrite,"}") && strcmp(yrite,"};") ) {
    if (!strcmp(yrite,suote) || !strcmp(yrite,suote1))
      goto LOPPU;
    /*korotetaan r:ää vain, jos sana päättyy pilkkuun*/
    if (yrite[strlen(yrite)-1] == ',')
      r++;
  }
  r = EI_KOMENTO;
 LOPPU:
  fclose(f);
  free(yrite);
  free(suote1);
  return r;
}
