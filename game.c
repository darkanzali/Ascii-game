#include <ncurses.h>

#include "struktury.h"
#include "game.h"
#include "okna.h"
#include "funkcje.h"

#define MAPFILE         "files/1.bin"

#define FLOOR           1
#define WALL            2
#define STOP_MONSTER    3
#define PLAYER          4
#define MONSTER         5
#define BOX             6
#define TELEPORT        7

void print_map( char filename[], Player *player, WINDOW *win );
void playGame( WINDOW *win ) {

    Player player;
    Monster *fmonster=NULL;

    wclear( win );
    printBorder( win );

    print_map( MAPFILE, &player, win );
    wrefresh( win );
    refresh();
    //load_player( &player );
    //load_monsters( fmonster );

    char c;
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
        //print_map( win, mapa, player, 5, 5 );
        wrefresh( win );
        refresh();
    }
}

void print_map( char filename[], Player *player, WINDOW *win ) {
    FILE *file;
    file = fopen( filename, "rb" );
    int hei, wid;
    int xg, yg;
    fread( &hei, sizeof( int ), 1, file );
    fread( &wid, sizeof( int ), 1, file );

    init_pair( FLOOR, COLOR_BLACK, COLOR_BLACK );
    init_pair( WALL, COLOR_WHITE, COLOR_WHITE );
    init_pair( PLAYER, COLOR_WHITE, COLOR_GREEN );
    init_pair( TELEPORT, COLOR_RED, COLOR_CYAN );
    init_pair( BOX, COLOR_WHITE, COLOR_MAGENTA );

    int i, j;
    Map_field m;
    for( i = 1; i <= hei; i++ ) {
        wmove( win, i, 1 );
        for( j = 1; j <= wid; j++ ) {
            fread( &m, sizeof( Map_field ), 1, file );
            switch( m.type ) {
                case WALL:
                    wattron( win, COLOR_PAIR( WALL ) );
                    wprintw( win, "W" );
                    wattroff( win, COLOR_PAIR( WALL ) );
                    break;
                case STOP_MONSTER:
                    wattron( win, COLOR_PAIR( FLOOR ) );
                    wprintw( win, "S" );
                    wattroff( win, COLOR_PAIR( FLOOR ) );
                    break;
                case PLAYER:
                    wattron( win, COLOR_PAIR( PLAYER ) );
                    wprintw( win, "G" );
                    wattroff( win, COLOR_PAIR( PLAYER ) );
                    break;
                case FLOOR:
                    wattron( win, COLOR_PAIR( FLOOR ) );
                    wprintw( win, "F" );
                    wattroff( win, COLOR_PAIR( FLOOR ) );
                    break;
                case MONSTER:
                    wattron( win, COLOR_PAIR( FLOOR ) );
                    wprintw( win, " " );
                    wattroff( win, COLOR_PAIR( FLOOR ) );
                    break;
                case TELEPORT:
                    wattron( win, COLOR_PAIR( TELEPORT ) );
                    wprintw( win, "T" );
                    wattroff( win, COLOR_PAIR( TELEPORT ) );
                    break;
                case BOX:
                    wattron( win, COLOR_PAIR( BOX ) );
                    wprintw( win, "Q" );
                    wattroff( win, COLOR_PAIR( BOX ) );
                    break;
                default:
                    wprintw( win, "E" );
                    break;
            }
        }
    }
    fread( &yg, sizeof( int ), 1, file );
    fread( &xg, sizeof( int ), 1, file );
    player -> x = xg;
    player -> y = yg;
    fclose( file );
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
