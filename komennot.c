#include "aliohjelmat.h"
#include "komennot.h"

kmnt_t enum_komennoksi(char *suote) {
  if (!strlen(suote)) //oli tyhjä
    return TUHJA;
  char haettava[strlen(suote)];
  strcpy(haettava, suote);
  kmnt_t r;
  FILE *f;
    

  /*Lopussa voi olla ';'. Se siis jätetään pois hausta.*/
  if(STR_END(haettava) == ';')
    STR_END(haettava) = '\0';

  /* 0, 1 ja 2 tulkitaan komentoina, koska käytän niitä siten*/
  if(strlen(haettava) == 1)
    if ((r = haettava[0]) >= '0' && r <= '2')
      return r - 48;

  if( !strcmp(haettava, "-a"))
    return LIITA;
  
  f = fopen(CSANAT_DIR"/komennot.h","r");
  if (!f)
    return EI_HEADERIA;
  char* yrite = calloc(H_MAX_SANA,1);
  if(!yrite)
    return VIRHE;

  /*haetaan kohta "enum kmnt_e {" */
  while (!feof(f)) {
    while( fscanf(f, "%s", yrite) == 1 && strcmp(yrite,"enum") );
    if (fscanf(f, "%s", yrite) == 1 && !strcmp(yrite,"kmnt_e"))
      if (fscanf(f, "%s", yrite) == 1 && !strcmp(yrite,"{"))
	goto ENUM_MAARITTELU_LOUTUI;
  }
  fclose(f);
  free(yrite);
  return EI_ENUMIA_HEADERISSA;
  
 ENUM_MAARITTELU_LOUTUI:
  /*haetaan annettua sanaa, lopussa voi olla pilkku */
  r = KMNT_ENUM_ALKU;
  char* haettava1 = calloc(H_MAX_SANA,1);
  sprintf(haettava1, "%s,", haettava);
  while( fscanf(f, "%s", yrite) == 1 && strcmp(yrite,"}") && strcmp(yrite,"};") ) {
    if (!strcmp(yrite,"NOLLA"))
      r = NOLLA;
    if (!strcmp(yrite,haettava) || !strcmp(yrite,haettava1))
      goto LOPPU;
    /*korotetaan r:ää vain, jos sana päättyy pilkuunn*/
    if (yrite[strlen(yrite)-1] == ',')
      r++;
  }
  r = ANNETTIIN_NIMI; //haetulla nimellä ei löytynyt komentoa
 LOPPU:
  fclose(f);
  free(yrite);
  free(haettava1);
  return r;
}
