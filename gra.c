#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "struktury.h"
#include "menu.h"
#include "okna.h"
#include "game.h"

#define true 1
#define false 0

int main() {
    /*****************/
    /* Inicjalizacja */
    /*****************/

    initscr(); // Inicjalizacja ncurses
    curs_set( 0 ); // Wyłączenie wyświetlania kursora
    cbreak(); // Wyłączenie oczekiwania na enter, teraz każdy bufor będzie wczytywał tylko jeden znak
    noecho(); // Wyłączenie wyświetlania wpisanego znaku

    int startx, starty; // Współrzędne lewego górnego rogu okna
    int startx2, starty2;
    int wx, wy; // Szerokość i wysokość terminala
    int hei = 20, wid = 80; // Wysokość i szerokość okienka
    int hei2 = 4, wid2 = 80;

    getmaxyx( stdscr, wy, wx ); // Pobranie wysokości i szerokości terminala

    if( wx < 80 || wy < 24 ) {
        endwin();
        printf( "Okno ma za mały rozmiar, minimalny wymagany to 24x80. Obecny %d %d.\n", wy, wx );
        exit( 1 );
    }
    if( has_colors() == FALSE ) {
        endwin();
        printf( "Wymagana jest obsluga kolorow w terminalu.\n" );
        exit( 1 );
	}
    start_color();

    WINDOW *mwin;
    WINDOW *textwin;

    starty = ( ( wy - hei ) / 2 ) - 2;
    startx = ( wx - wid ) / 2;
    starty2 = starty + hei;
    startx2 = startx;
    mwin = newwin( hei, wid, starty, startx );
    textwin = newwin( hei2, wid2, starty2, startx2 );
    refresh();
    printBorder( mwin );
    printBorder( textwin );
    wmove( textwin, 1, 2 );
    wprintw( textwin, "Autor: Kamil Poniatowski 2015" );
    wrefresh( textwin );

    /************************/
    /* Deklaracje zmiennych */
    /************************/

    int endGame = false;

    /*******/
    /* Gra */
    /*******/

    while( 1 ) {
                                    // 1 Nowa gra
        switch( menu( mwin ) ) {    // 2 Wczytaj grę
            case 1:                 // 3 Konwersja map
                wclear( textwin );
                printBorder( textwin );
                wrefresh( textwin );
                playGame( mwin, textwin );   // 4 Wyjście
                break;
            case 2:
                endGame = false;
                break;
            case 3:
                endGame = false;
                break;
            case 4:
                endGame = true;
                break;
            default:
                endwin();
                printf( "Oops, game crashed!" );
                exit( 1 );
                break;
        }

        if( endGame )
            break;
    }

    endwin();
    return 0;
}
