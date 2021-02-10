#include <unistd.h>
#include <dirent.h>
#include "aliohjelmat.h"
#include "komennot.h"
#include <omastring.h>
#define MAXLEN_KMNT_MAIN 2000

strlista* auki_oleva;
void tulosta_auki_oleva(void);
int exist_dir(char*);

int main(int argc, char** argv) {
  auki_oleva = NULL;
  strlista* knnot = NULL;
  snlista *l = NULL;
  krrs_tdt k = {NULL};
  snlista* vanha_kl = NULL;
  char *komento;
  snlista* vanha_l;
  char liita_sanat = 0;
  if(! (komento = calloc(MAXLEN_KMNT_MAIN,1)) )
    return 1;
  
  /*Otetaan komennot komentoriviltä, jos annettu*/
  if(argc > 1) {
    for (int i=1; i<argc; i++) {
      char* apu;
      if ( !(apu = calloc(strlen(argv[i])+1,1)) )
	return 1;
      strcpy(apu, argv[i]);
      knnot = _ylisaa(knnot, apu);
    }
    knnot = _yalkuun(knnot);
    goto KOMENTO_VALMIINA;
  }
  
  /*Kysytään komento ja pilkotaan se sanoiksi*/
  while (1) {
    if(!fgets(komento, MAXLEN_KMNT_MAIN, stdin))
      puts("Lukeminen ei onnistunut");
    komento[strlen(komento)-1] = '\0';
    if (!(knnot = pilko_sanoiksi(komento)))
      return 1;

  KOMENTO_VALMIINA:;
    kmnt_t kmnt;
    while(knnot) {
      kmnt = enum_komennoksi(knnot->str);
      switch(kmnt) {
      case CD:
	/*voidaan kutsua (yksin tai tyhjällä)->HOME
	  tai kansion nimellä -> KANSIO*/
	if ( !(knnot->seur) || !strcmp(knnot->str, "CD;") ) //tähän nargin == 1
	  chdir(HOME);
	else {
	  knnot = _strpoista1(knnot, 1);
	  if( strlen(knnot->str)>0 ) {
	    if( !exist_dir(knnot->str) ) {
	      printf("Ei ole kansioa nimeltä \"%s\"\n", knnot->str);
	    } else {
	      chdir(knnot->str);
	    }
	  } else { 
	    printf("Varoitus: \"CD\" + \"\" tulkittiin muotoon \"CD;\"\n");
	    chdir(HOME);
	  }
	}
	break;
      case ECHO:
	if ( !(knnot->seur) || !strcmp(knnot->str, "ECHO;") ) {
	  puts("");
	} else { //nargin > 1
	  knnot = _strpoista1(knnot, 1);
	  puts(knnot->str);
	}
	break;
      case EI_HEADERIA:
	fprintf(stderr, "Virhe: ei voi lukea tiedostoa komennot.h\n");
	break;
      case EI_ENUMIA_HEADERISSA:
	fprintf(stderr, "Virhe: tiedostossa \"komennot.h\" ei ole enumia \"kmnt_e\"\n");
	break;
      case KANSIO:;
	int koko = 100;
	char *kansio = malloc(koko);
	while(!getcwd(kansio, koko))
	  kansio = realloc(kansio, koko+=100);
	puts(kansio);
	free(kansio);
	break;
      case LISTAA:
	_strpoista_kaikki(listaa(&knnot));
	break;
      case LIITA:
	liita_sanat = 1;
      case VIRHE:
	fprintf(stderr, "Virhe: varmaankin jokin calloc epäonnistui\n");
	break;
      case 0:
	snpuorauta(k.l);
	break;
      case 1:
	_ypoista_kaikki(k.l, snvapautus);
	k.l = NULL; // nyt kierros alustetaan l:stä, koska k.l ei ole saatavilla
	break;
      case 2:
	break;
      case NYT:
	_strtulosta((strlista*)_yalkuun(auki_oleva));
	putchar('\n');
	break;
      case OSATUT:
	TOISTA(k.n_osattuja)
	  printf("%s = %s\n", snnouda(k.l, k.osatut[tmp])->sana, snnouda(k.l, k.osatut[tmp])->kaannos);
	break;
      case POISTU:
      case EXIT:
	goto LOPPU;
      case TALLENNA:;
	char n = tallenna(k.l, knnot);
	TOISTA(n)
	  knnot = _strpoista1(knnot, 1);
	break;
      case TUHJA:
	k.l = _ypoista(k.l, k.osatut, k.n_osattuja, snvapautus);
	break;
      case ANNETTIIN_NIMI:
      default:
	/*Avataan tiedosto ja vapautetaan kierros ja alustetaan uusi*/
	vanha_l = l;
	if ( knnot->str && !(l = avaa(knnot->str)) ) {
	  printf("Ei voi avata tiedostoa \"%s\"\n", knnot->str);
	  kmnt = -100; //ei mennä kierrokselle
	} else {
	  _ypoista_kaikki(k.l, snvapautus);
	  k.l = NULL;
	  /*auki_oleva on oletuksena lopussa*/
	  if (!liita_sanat) {
	    _ypoista_kaikki(vanha_l, snvapautus);
	    _strpoista_kaikki((strlista*)_yalkuun(auki_oleva));
	    auki_oleva = NULL;
	  } else {
	    auki_oleva = _strlisaa_kopioiden(auki_oleva, knnot->str);
	  }
	}
      }//endswitch
      knnot = _strpoista1(knnot, 1);
    }
    if (kmnt < 0)
      continue;

    /*Kmnt on positiivinen eli mennään kierrokselle*/
    /*Vapautetaan ensin mahdollinen vanha kierrostieto*/
    if(k.suotteet) {
      TOISTA(N_SUOTTEITA_KIERROKSELLA)
	free(k.suotteet[tmp]);
      free(k.suotteet);
      free(k.osatut);
    }

    /*Tehdään uusi kierrostieto
      Listana käytetään samaa kuin ennen eli k.l, jos mahdollista.
      Jos ei, käytetään koko listaa l*/
    vanha_kl = k.l;
    if (k.l) {
      k = alusta_kierros(vanha_kl);
      _ypoista_kaikki(vanha_kl, snvapautus);
    } else
      k = alusta_kierros(l);
    if (!k.osatut) {
      printf("Kierroksen alustaminen ei onnistunut\n");
      return 1;
    }

    k.l = sekoita(k.l);
    k = kierros(k);
    printf("Kierrokselta %i/%i. Jäljellä %i/%i. (",	\
	   k.n_osattuja, _ylaske(k.l),		       	\
	   _ylaske(k.l)-k.n_osattuja, _ylaske(k.l));
    tulosta_auki_oleva();
    printf(")\n");
  }
  /*Vapautetaan ohjelman sulkeutuessa käytetyt asiat.*/
 LOPPU:
  if(knnot)
    _strpoista_kaikki(knnot);
  free(komento);
  if(k.suotteet) {
    TOISTA(N_SUOTTEITA_KIERROKSELLA)
      free(k.suotteet[tmp]);
    free(k.suotteet);
    free(k.osatut);
  }
  _ypoista_kaikki(k.l, snvapautus);
  _ypoista_kaikki(l, snvapautus);
  /*auki_oleva on oletuksena lopussa*/
  _strpoista_kaikki((strlista*)_yalkuun(auki_oleva));
  return 0;
}

void tulosta_auki_oleva() {
  if (!auki_oleva)
    return;
  auki_oleva = _yalkuun(auki_oleva);
  goto KIERROS1;
  do {
    auki_oleva = auki_oleva->seur;
  KIERROS1:
    printf("%s ", auki_oleva->str);
  } while(auki_oleva->seur);
  printf("\b");
}

int exist_dir(char* kansio) {
  DIR *d = opendir(kansio);
  if(!d)
    return 0;
  closedir(d);
  return 1;
}
