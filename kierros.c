#include "aliohjelmat.h"
#include "kierroskomennot.h"
#include "omastring.h"

int kierros_maxpit(snlista* l) {
  int i;
  int maxpit = 0;
  int kokeilu;
  int snlistpit = _ylaske(l);
  for (i=0; i<snlistpit; i++)
    if ((kokeilu = nmerkit(snnouda(l,i)->sana)) > maxpit)
      maxpit = kokeilu;
  return maxpit;
}

krrs_tdt kierros(krrs_tdt k) {
  int i = 0;
  char **suote = k.suotteet;
  int suotteen_paikka = kierros_maxpit(k.l) + 3;
  while(i < _ylaske(k.l)) {
    int valeja = suotteen_paikka - nmerkit(snnouda(k.l,i)->sana);

    /*Kysytään sana*/
    printf("%s", snnouda(k.l,i)->sana);
    TOISTA(valeja)
      printf(" ");
    fgets(*suote, MAXLEN_STDIN, stdin);

    /*Poistetaan lopusta rivinvaihto, jos syöte mahtui kokonaan*/
    if ((*suote)[strlen(*suote)-1] != '\n') {
      printf("Koko syöte ei mahtunut\n");
    } else {
      (*suote)[strlen(*suote)-1] = 0;
    }

    /*Komento?*/
    strlista* suotteet_pilkottuna = pilko_sanoiksi(*suote);
    kierroskmnt_t kmnt = kierroskomennoksi(suotteet_pilkottuna->str);
    switch(kmnt) {
    case EI_ENUMIA_HEADERISSA:
      fprintf(stderr, "Virhe: Ei enumia headerissa\n");
      break;
    case EXIT:
      return k;
    case KORJAA:
      korjaa(k.l, &suotteet_pilkottuna);
      break;
    case L_ARROW:
      i--;
      break;
    case OLI_VAARIN:
      TOISTA(k.n_osattuja)
	if (k.osatut[tmp] == i) {
	  (k.n_osattuja)--;
	  for(int j=tmp; j<k.n_osattuja-tmp; j++)
	    k.osatut[j] = k.osatut[j+1];
	}
      break;
    case RISUAITA:
      printf("%i/%i: %i osattua\n", i, _ylaske(k.l), k.n_osattuja);
      break;
    case TAMA_OIKEIN:
      TOISTA(k.n_osattuja) //Oliko jo osatuissa
	if(k.osatut[tmp] == i) {
	  i++;
	  break;
	}
      k.osatut[(k.n_osattuja)++] = i++;
      break;
    case VIRHE:
      fprintf(stderr, "Virhe: varmaan jokin calloc epäonnistui\n");
      break;
    case YKSI:
      if (i == 0) {
	printf("Ei ole edellistä\n");
	break;
      }
      TOISTA(k.n_osattuja) //Oliko jo osatuissa
	if(k.osatut[tmp] == i-1) {
	  i++;
	  break;
	}
      k.osatut[(k.n_osattuja)++] = i-1;
      break;
    default:
      /*Oliko sana oikein*/
      if (strcmp(*suote, snnouda(k.l,i)->kaannos) == 0) {
	k.osatut[(k.n_osattuja)++] = i++;
      } else {
	for (int tmp=0; tmp<suotteen_paikka; tmp++)
	  printf(" ");
	printf("= %s\n",snnouda(k.l,i++)->kaannos);
      }
    } //endswitch
    suote++;
    _strpoista_kaikki(suotteet_pilkottuna);
  }
  return k;
}
