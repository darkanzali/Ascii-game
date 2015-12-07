#include <ncurses.h>

#include "struktury.h"
#include "game.h"
#include "okna.h"
#include "funkcje.h"

#define FLOOR 0
#define WALL 1
#define PLAYER 2

//void load_map( Mapa *map );
//void print_map( WINDOW *win, Mapa mapa[5][5], Player player, int height, int width );
void playGame( WINDOW *win ) {

    Map *fmap=NULL;
    Player player;
    Monster *fmonster=NULL;

    load_map( fmap );
    load_player( &player );
    load_monsters( fmonster );

    wclear( win );
    printBorder( win );
    char c;
    wmove( win, 1, 1 );
    while( 1 ) {
        if( kbhit() ) {
            c = getch();

            if( c == '\033' )
            {
                getch();
                c=getch();
                switch( c ) {
                    case 'A': // Up

                        break;
                    case 'B': // Down

                        break;
                    case 'C': // Right

                        break;
                    case 'D': // Left

                        break;
                    default:
                        break;
                }
            }
        } // kbhit
        printBorder( win );
        //print_map( win, mapa, player, 5, 5 );
        wrefresh( win );
        refresh();
    }
}

/*void print_map( WINDOW *win, Mapa mapa[5][5], Player player, int height, int width ) {
    init_pair( FLOOR, COLOR_BLACK, COLOR_WHITE );
    init_pair( WALL, COLOR_BLUE, COLOR_RED );
    init_pair( PLAYER, COLOR_RED, COLOR_BLUE );

    int i, j;
    for( i = 0; i < height; i++ ) {
        wmove( win, i+1, 1);
        for( j = 0; j < width; j++ ) {
            if( mapa[ i ][ j ].type == FLOOR ) {
                wprintw( win, " " );
            }
            if( mapa[ i ][ j ].type == WALL ) {
                wattron( win, COLOR_PAIR( WALL ) );
                wprintw( win, " " );
                wattroff( win, COLOR_PAIR( WALL ) );
            }
            if( mapa[ i ][ j ].type == PLAYER ) {
                wattron( win, COLOR_PAIR( PLAYER ) );
                wprintw( win, "P" );
                wattroff( win, COLOR_PAIR( PLAYER ) );
            }
        }
    }
}

void load_map( Mapa *map ) {
    FILE *file;
    file = fopen( "map/main.dat", "rb" );

    MapSettings settings;
    fread( &settings, sizeof( MapSettings ), 1, file );
    fclose( file );

}*/
