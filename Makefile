default: gra

gra.o: gra.c
	gcc -c gra.c -o gra.o

okna.o: okna.c
	gcc -c okna.c -o okna.o

menu.o: menu.c
	gcc -c menu.c -o menu.o

game.o: game.c
	gcc -c game.c -o game.o

funkcje.o: funkcje.c
	gcc -c funkcje.c -o funkcje.o

colors.o: colors.c
	gcc -c colors.c -o colors.o

gra: gra.o okna.o menu.o game.o funkcje.o colors.o
	gcc -O2 gra.o okna.o menu.o game.o funkcje.o colors.o -o gra -Wall -Wparentheses -lncursesw -lm
	-rm *.o

clean:
	-rm -f *.o
	-rm -f gra
