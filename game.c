#include <ncurses.h>

#include "struktury.h"
#include "game.h"
#include "okna.h"
#include "funkcje.h"

#define FLOOR 0
#define WALL 1
#define PLAYER 2

void init_map( Mapa mapa[5][5], int height, int width );
void print_map( WINDOW *win, Mapa mapa[5][5], Player player, int height, int width );
void playGame( WINDOW *win ) {

    Mapa mapa[ 5 ][ 5 ];
    init_map( mapa, 5, 5 );

    mapa[ 0 ][ 0 ].type = 1;
    mapa[ 0 ][ 1 ].type = 1;
    mapa[ 0 ][ 2 ].type = 1;
    mapa[ 0 ][ 3 ].type = 1;
    mapa[ 0 ][ 4 ].type = 1;

    mapa[ 1 ][ 0 ].type = 1;
    mapa[ 1 ][ 1 ].type = PLAYER;
    mapa[ 1 ][ 4 ].type = 1;

    mapa[ 2 ][ 0 ].type = 1;
    mapa[ 2 ][ 4 ].type = 1;

    mapa[ 3 ][ 0 ].type = 1;
    mapa[ 3 ][ 4 ].type = 1;

    mapa[ 4 ][ 0 ].type = 1;
    mapa[ 4 ][ 1 ].type = 1;
    mapa[ 4 ][ 2 ].type = 1;
    mapa[ 4 ][ 3 ].type = 1;
    mapa[ 4 ][ 4 ].type = 1;

    Player player;
    player.x = 1;
    player.y = 1;


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
                    case 'A':
                        if( mapa[ player.y - 1 ][ player.x ].type == FLOOR ) {
                            mapa[ player.y - 1 ][ player.x ].type = PLAYER;
                            mapa[ player.y ][ player.x ].type = FLOOR;
                            player.y--;
                        }
                        break;
                    case 'B':
                        if( mapa[ player.y + 1 ][ player.x ].type == FLOOR ) {
                            mapa[ player.y + 1 ][ player.x ].type = PLAYER;
                            mapa[ player.y ][ player.x ].type = FLOOR;
                            player.y++;
                        }
                        break;
                    case 'C':
                        if( mapa[ player.y ][ player.x + 1 ].type == FLOOR ) {
                            mapa[ player.y ][ player.x + 1 ].type = PLAYER;
                            mapa[ player.y ][ player.x ].type = FLOOR;
                            player.x++;
                        }
                        break;
                    case 'D':
                        if( mapa[ player.y ][ player.x - 1 ].type == FLOOR ) {
                            mapa[ player.y ][ player.x - 1 ].type = PLAYER;
                            mapa[ player.y ][ player.x ].type = FLOOR;
                            player.x--;
                        }
                        break;
                }
            }
        } // kbhit
        printBorder( win );
        print_map( win, mapa, player, 5, 5 );
        wrefresh( win );
        refresh();
    }
}

void print_map( WINDOW *win, Mapa mapa[5][5], Player player, int height, int width ) {
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

void init_map( Mapa mapa[5][5], int height, int width ) {
    int i, j;
    for( i = 0; i < height; i++ ) {
        for( j = 1; j < width; j++ ) {
            mapa[ i ][ j ].type = FLOOR;
            mapa[ i ][ j ].id = 0;
        }
    }
}
