#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

#include "struktury.h"
#include "menu.h"
#include "okna.h"
#include "game.h"

int main() {
    /*****************/
    /* Inicjalizacja */
    /*****************/

    initscr(); // Inicjalizacja ncurses
    curs_set( 0 ); // Wyłączenie wyświetlania kursora
    cbreak(); // Wyłączenie oczekiwania na enter, teraz każdy bufor będzie wczytywał tylko jeden znak
    noecho(); // Wyłączenie wyświetlania wpisanego znaku

    int startx, starty; // Współrzędne lewego górnego rogu okna
    int wx, wy; // Szerokość i wysokość terminala
    int hei = 20, wid = 80; // Wysokość i szerokość okienka

    getmaxyx( stdscr, wy, wx ); // Pobranie wysokości i szerokości terminala

    if( wx < 80 || wy < 20 ) {
        endwin();
        printf( "Okno ma za mały rozmiar, minimalny wymagany to 20x30. Obecny %d %d.\n", COLS, LINES );
        exit( 1 );
    }
    if( has_colors() == FALSE ) {
        endwin();
        printf( "Wymagana jest obsluga kolorow w terminalu.\n" );
        exit( 1 );
	}
    start_color();

    WINDOW *mwin;

    starty = ( wy - hei ) / 2;
    startx = ( wx - wid ) / 2;
    mwin = newwin( hei, wid, starty, startx );
    refresh();
    printBorder( mwin );

    /************************/
    /* Deklaracje zmiennych */
    /************************/

    //int choice = 0; // Wybór z menu
    int endGame = 0;
    char map_file_sett[] = "map/settings.bin";

    /*******/
    /* Gra */
    /*******/

    while( 1 ) {
        //choice = menu( mwin );      // 1 Nowa gra
                                    // 2 Wczytaj grę
        switch( menu( mwin ) ) {          // 3 Konwersja map
            case 1:                 // 4 Wyjście
                playGame( mwin );
                break;
            case 2:
                endGame = 0;
                break;
            case 3:
                endGame = 0;
                break;
            case 4:
                endGame = 1;
                break;
            default:
                endwin();
                printf( "Oops, game crashed!" );
                exit( 1 );
                break;
        }

        if( endGame == 1 )
            break;
    }

    endwin();
    return 0;
}
