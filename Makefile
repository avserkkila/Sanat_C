LHD_TIED=main.c snlista.c kierros.c aliohjelmat.c komennot.c kierroskomennot.c tallenna.c listaa.c
incdir=-I/usr/include/SDL2 -I/home/antterkk/c
libdir=-L/home/antterkk/c
libs=-lSDL2 -lSDL2_ttf -llistat -lomastring -lm

ULOSNIMI=sanaohj

all: kaanna aja

kaanna:
	gcc -gdwarf-2 -g3 -Wall -o ${ULOSNIMI} ${LHD_TIED} ${incdir} ${libdir} ${libs}

aja:
	./${ULOSNIMI}
