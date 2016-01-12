/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <stdio.h>
#include <stdlib.h>
#include <ncursesw/ncurses.h>
#include <locale.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "definicje.h"
#include "struktury.h"
#include "menu.h"
#include "okna.h"
#include "game.h"
#include "colors.h"

void info_help( WINDOW *win );
void init_windows( Windows windows );
void print_ending( WINDOW *win );
int start_new_game( WINDOW *win );
int main() {
    /*****************/
    /* Inicjalizacja */
    /*****************/

    setlocale( LC_CTYPE, "" );
    initscr();
    curs_set( 0 );
    cbreak();
    noecho();
    keypad( stdscr, TRUE );
    start_color();

    int startx, starty;
    int startx2, starty2;
    int startx3, starty3;
    int wx, wy;
    int hei = 22, wid = 82;
    int hei2 = 4, wid2 = wid;
    int hei3 = 8, wid3 = 12;
    int hei4 = 11, wid4 = wid3;
    int hei5 = 3, wid5 = wid3;
    int world;

    getmaxyx( stdscr, wy, wx );

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
    
    start_color();

    WINDOW *mwin;
    WINDOW *twin;
    WINDOW *rwin;
    WINDOW *ewin;
    WINDOW *dwin;

    starty = ( ( wy - hei ) / 2 ) - 2;
    startx = ( ( wx - wid ) / 2 ) - 12;
    starty2 = starty + hei;
    startx2 = startx;
    starty3 = starty;
    startx3 = startx + wid;
    mwin = newwin( hei, wid, starty, startx );
    twin = newwin( hei2, wid2, starty2, startx2 );
    rwin = newwin( hei3, wid3, starty3, startx3 );
    ewin = newwin( hei4, wid4, starty3 + hei3, startx3 );
    dwin = newwin( hei5, wid5, starty3 + hei3 + hei4, startx3 );

    Windows windows;
    windows.mwin = mwin;
    windows.twin = twin;
    windows.rwin = rwin;
    windows.ewin = ewin;
    windows.dwin = dwin;

    bool endGame = false;

    while( true ) {
        init_windows( windows );
        switch( menu( mwin ) ) {    // 1 Nowa gra // 2 Wczytaj grę
            case 1:                 // 3 Pomoc // 4 Wyjście
                world = 1;
                if( !start_new_game( twin ) )
                    break;
                do {
                    world = playGame( world, windows );
                } while( world != -1 && world != -2 );
                init_windows( windows );
                if( world == -2 )
                    print_ending( mwin );
                endGame = false;
                break;
            case 2:
                world = -1;
                do {
                    world = playGame( world, windows );
                } while( world != -1 && world != -2 );
                init_windows( windows );
                if( world == -2 )
                    print_ending( mwin );
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
    wprintw( win, "Potwory są koloru różowego i są oznaczone pierwszą literą nazwy. Będziesz grał" );
    wmove( win, 5, 2 );
    wprintw( win, "na kolejnych mapach zabijając coraz mocniejsze potwory. Skrzynie ze zbrojami," );
    wmove( win, 6, 2 );
    wprintw( win, "broniami, potionami oraz kluczami do portali są ustawione na mapach i oznaczone" );
    wmove( win, 7, 2 );
    wprintw( win, "symbolem &. W skrzyniach znajdują się klucze wymagane żeby otworzyć drzwi" );
    wmove( win, 8, 2 );
    wprintw( win, "oznaczone literą L, do portalu do następnego poziomu. Po prawej stronie razem" );
    wmove( win, 9, 2 );
    wprintw( win, "z informacjami o zbroi itd. jest napisane czy posiadasz klucz do aktualnego" );
    wmove( win, 10, 2 );
    wprintw( win, "poziomu. To teraz trochę informacji o głównej postaci. Jesteś kosmicznym klocem" );
    wmove( win, 12, 2 );
    wprintw( win, "który leciał po wszechświecie swoim statkiem kosmicznym i rozmawiał przez" );
    wmove( win, 13, 2 );
    wprintw( win, "telefon trzymając go przy uchu, niestety wpadł w turbulencje i telefon wypadł" );
    wmove( win, 14, 2 );
    wprintw( win, "mu z ręki, gdy go szukał nie zauważył że leci prosto w kosmiczny teleport." );
    wmove( win, 15, 2 );
    wprintw( win, "Został przeniesiony na planetę na której jest pełno potworów które chcą go" );
    wmove( win, 16, 2 );
    wprintw( win, "zabić, aby wydostać się z niej musi przejść wszystkie poziomy. " );
    wmove( win, 17, 2 );
    wprintw( win, "" );
    wmove( win, 18, 2 );
    wprintw( win, "Powodzenia!" );
    wmove( win, 20, 2 );
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
    wclear( windows.ewin );
    wclear( windows.dwin );
    printBorder( windows.mwin );
    printBorder( windows.twin );

    wmove( windows.twin, 1, 2 );
    wprintw( windows.twin, "Gra ASCII, \"Przygody kloca\"." );
    wmove( windows.twin, 2, 2 );
    wprintw( windows.twin, "© Kamil Poniatowski 2015" );

    wrefresh( windows.mwin );
    wrefresh( windows.twin );
    wrefresh( windows.rwin );
    wrefresh( windows.ewin );
    wrefresh( windows.dwin );
}

int start_new_game( WINDOW *win ) {
    int n = 0;
    struct dirent *d;
    DIR *dir = opendir( "save" );

    if( dir == NULL ) {
        struct stat st = {0};
        if( stat( "save", &st ) == -1 ) {
            mkdir( "save", 0755 );
        } else {
            endwin();
            printf( "Save directory corrupted" );
            exit( 1 );
        }
    }

    while ( ( d = readdir( dir ) ) != NULL )
        if( ++n > 2 )
            break;

    closedir( dir );
    if( n <= 2 )
        return true;
    else {
        wclear( win );
        printBorder( win );
        wmove( win, 1, 1 );
        wprintw( win, "Istnieje zapisana gra, usunąć zapis i zacząć grę od nowa? (t/n)" );
        wrefresh( win );
        char c;
        do {
            c = getchar();
        } while( c != 't' && c != 'n' );
        if( c == 'n' ) {
            return false;
        } else {
            dir = opendir( "save" );
            char *name;
            while( ( d = readdir( dir ) ) != NULL ) {
                if( strcmp( d -> d_name, "." ) != 0 && strcmp( d -> d_name, ".." ) != 0 ) {
                    name = calloc( ( strlen( d -> d_name ) + 5 ) + 2, sizeof( char ) );
                    strcpy( name, "save/" );
                    strcat( name, d -> d_name );
                    if( remove( name ) == -1 ) {
                        endwin();
                        printf( "Deleting file failed (%s)", name );
                        exit( 1 );
                    }
                    free( name );
                }
            }
            return true;
        }
    }

    return true;
}


void print_ending( WINDOW* win ) {
    wclear( win );
    printBorder( win );
    wmove( win, 1, 1 );
    wprintw( win, "Gratulacje!" );
    wmove( win, 2, 1 );
    wprintw( win, "To już koniec gry. Mam nadzieję że Ci się podobała." );
    wmove( win, 3, 1 );
    wprintw( win, "Kloc uciekł z dziwnego świata w którym się zagubił" );
    wmove( win, 4, 1 );
    wprintw( win, "i żył długo i szczęśliwie." );
    wmove( win, 7, 1 );
    wprintw( win, "Aby wrócić do menu wciśnij m" );
    wrefresh( win );
    char c;
    do {
        c = getchar();
    } while( c != 'm' );
}
