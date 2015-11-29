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

gra: gra.o okna.o menu.o game.o funkcje.o
	gcc gra.o okna.o menu.o game.o funkcje.o -o gra -lncurses
	-rm *.o

clean:
	-rm -f *.o
	-rm -f gra
