/*kommenteissa ei saa olla enum-termin tai jonkin sanaston nimeä*/

typedef enum kmnt_e {
  KMNT_ENUM_ALKU = -100,
  CD,
  ECHO,
  EI_HEADERIA,
  EI_ENUMIA_HEADERISSA,
  EXIT,
  LIITA,
  KANSIO,
  LISTAA,
  NYT,
  OSATUT,
  POISTU,
  TALLENNA,
  VIRHE,

  NOLLA = 0,
  KAIKKI,
  ALA_POISTA,
  TUHJA,
  ANNETTIIN_NIMI,
} kmnt_t;

kmnt_t enum_komennoksi(char *str);
