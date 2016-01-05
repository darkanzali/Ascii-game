#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include <locale.h>

#include "definicje.h"
#include "struktury.h"
#include "menu.h"
#include "okna.h"
#include "game.h"
#include "colors.h"

void info_help( WINDOW *win );
void init_windows( Windows windows );
int main() {
    /*****************/
    /* Inicjalizacja */
    /*****************/

    setlocale( LC_CTYPE, "" );
    initscr(); // Inicjalizacja ncurses
    curs_set( 0 ); // Wyłączenie wyświetlania kursora
    cbreak(); // Wyłączenie oczekiwania na enter, teraz każdy bufor będzie wczytywał tylko jeden znak
    noecho(); // Wyłączenie wyświetlania wpisanego znaku
    keypad( stdscr, TRUE ); // Włączenie pobierania specjalnych klawiszy
    start_color();

    int startx, starty; // Współrzędne lewego górnego rogu okna
    int startx2, starty2;
    int startx3, starty3;
    int wx, wy; // Szerokość i wysokość terminala
    int hei = 22, wid = 82; // Wysokość i szerokość okienka głównego
    int hei2 = 4, wid2 = wid; // Wysokość i szerokość okienka tekstowego
    int hei3 = 8, wid3 = 12;
    int world; // Aktualny świat

    getmaxyx( stdscr, wy, wx ); // Pobranie wysokości i szerokości terminala

    if( wx < 94 || wy < 26 ) {
        endwin();
        printf( "Okno ma za mały rozmiar, minimalny wymagany to 26x94. Obecny %d %d.\n", wy, wx );
        exit( 1 );
    }
    if( has_colors() == false ) {
        endwin();
        printf( "Wymagana jest obsługa kolorów w terminalu.\n" );
        exit( 1 );
	}
    if( can_change_color() == false ) {
        endwin();
        printf( "Terminal musi obslugiwać 256 kolorów.\n" );
        exit( 1 );
    }
    start_color();

    WINDOW *mwin;
    WINDOW *twin;
    WINDOW *rwin;

    starty = ( ( wy - hei ) / 2 ) - 2;
    startx = ( ( wx - wid ) / 2 ) - 12;
    starty2 = starty + hei;
    startx2 = startx;
    starty3 = starty;
    startx3 = startx + wid;
    mwin = newwin( hei, wid, starty, startx );
    twin = newwin( hei2, wid2, starty2, startx2 );
    rwin = newwin( hei3, wid3, starty3, startx3 );

    Windows windows;
    windows.mwin = mwin;
    windows.twin = twin;
    windows.rwin = rwin;

    /************************/
    /* Deklaracje zmiennych */
    /************************/

    bool endGame = false;

    /*******/
    /* Gra */
    /*******/

    while( true ) {
        init_windows( windows );
        switch( menu( mwin ) ) {    // 1 Nowa gra // 2 Wczytaj grę
            case 1:                 // 3 Pomoc // 4 Wyjście
                world = 1;
                goto gramy;
                break;
            case 2:
                world = 1; // TODO: wczytywanie wybranego świata
                gramy:
                wclear( twin );
                printBorder( twin );
                wrefresh( twin );
                do {
                    world = playGame( world, windows );
                } while( world != -1 );
                endGame = false;
                break;
            case 3:
                info_help( mwin );
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
void info_help( WINDOW *win) {
    wclear( win );
    printBorder( win );
    wmove( win, 1, 2 );
    wprintw( win, "Witaj w terminalowej grze ASCII, poniżej znajdziesz informacje jak grać" );
    wmove( win, 3, 2 );
    wprintw( win, "Twoja postać jest oznaczona symbolem @" );
    wmove( win, 4, 2 );
    wprintw( win, "Potwory są koloru brązowego i są oznaczone pierwszą literą nazwy" );
    wmove( win, 5, 2 );
    wprintw( win, "Będziesz grał na kolejnych mapach zabijając coraz mocniejsze potwory" );
    wmove( win, 6, 2 );
    wprintw( win, "Skrzynie z itemami oraz kluczami do portali są rozstawione po mapie" );
    wmove( win, 7, 2 );
    wprintw( win, "Pamiętaj żeby zawsze otworzyć wszystkie skrzynie, mogą tam znajdować się" );
    wmove( win, 8, 2 );
    wprintw( win, "potrzebne rzeczy do przejścia dalszych etapów. Nie martw się jak czegoś" );
    wmove( win, 9, 2 );
    wprintw( win, "zapomnisz zawsze możesz wrócić do poprzednich poziomów" );
    wmove( win, 11, 2 );
    wprintw( win, "Aby wrócić do menu kliknij m " );
    wrefresh( win );
    int c;
    do {
        c = getch();
    } while( c != 'm' );
}

void init_windows( Windows windows ) {
    wclear( windows.mwin );
    wclear( windows.twin );
    wclear( windows.rwin );
    printBorder( windows.mwin );
    printBorder( windows.twin );

    wmove( windows.twin, 1, 2 );
    wprintw( windows.twin, "Gra ASCII" );
    wmove( windows.twin, 2, 2 );
    wprintw( windows.twin, "© Kamil Poniatowski 2015" );

    wrefresh( windows.mwin );
    wrefresh( windows.twin );
    wrefresh( windows.rwin );
}
