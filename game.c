#include <ncursesw/ncurses.h>

#include "struktury.h"
#include "game.h"
#include "okna.h"

void print_map( char filename[], Player *player, WINDOW *win, WINDOW *twin );
int playGame( int world, WINDOW *win, WINDOW *twin ) {

    Player player;
    Monster *fmonster=NULL;
    bool goMenu = false;

    wclear( win );
    printBorder( win );

    print_map( MAPFILE, &player, win, twin );
    wrefresh( win );
    refresh();
    //load_player( &player );
    //load_monsters( fmonster );

    init_pair( FLOOR, COLOR_BLACK, COLOR_BLACK );
    init_pair( WALL, COLOR_WHITE, COLOR_WHITE );
    init_pair( PLAYER, COLOR_WHITE, COLOR_GREEN );
    init_pair( TELEPORT, COLOR_RED, COLOR_CYAN );
    init_pair( BOX, COLOR_WHITE, COLOR_MAGENTA );

    int c;
    while( !goMenu ) {
        if( kbhit() ) {
            c = getch();
            switch( c ) {
                case 'q':
                    c = getch();
                    if( c == 't' )
                        goMenu = true;
                    break;
                case KEY_UP: // Up
                    wmove( win, player.y - 1, player.x );
                    if( ( winch( win ) & A_CHARTEXT ) != 87 ) {
                        player.y--;
                        wattron( win, COLOR_PAIR( PLAYER ) );
                        wprintw( win, "G" );
                        wattroff( win, COLOR_PAIR( PLAYER ) );

                        wmove( win, player.y + 1, player.x );

                        wattron( win, COLOR_PAIR( FLOOR ) );
                        wprintw( win, "F" );
                        wattroff( win, COLOR_PAIR( FLOOR ) );
                    }
                    break;
                case KEY_DOWN: // Down
                    wmove( win, player.y + 1, player.x );
                    if( ( winch( win ) & A_CHARTEXT ) != 87 ) {
                        player.y++;
                        wattron( win, COLOR_PAIR( PLAYER ) );
                        wprintw( win, "G" );
                        wattroff( win, COLOR_PAIR( PLAYER ) );

                        wmove( win, player.y - 1, player.x );

                        wattron( win, COLOR_PAIR( FLOOR ) );
                        wprintw( win, "F" );
                        wattroff( win, COLOR_PAIR( FLOOR ) );
                    }
                    break;
                case KEY_RIGHT: // Right
                    wmove( win, player.y, player.x + 1 );
                    if( ( winch( win ) & A_CHARTEXT ) != 87 ) {
                        player.x++;
                        wattron( win, COLOR_PAIR( PLAYER ) );
                        wprintw( win, "G" );
                        wattroff( win, COLOR_PAIR( PLAYER ) );

                        wmove( win, player.y, player.x - 1 );

                        wattron( win, COLOR_PAIR( FLOOR ) );
                        wprintw( win, "F" );
                        wattroff( win, COLOR_PAIR( FLOOR ) );
                    }
                    break;
                case KEY_LEFT: // Left
                    wmove( win, player.y, player.x - 1 );
                    if( ( winch( win ) & A_CHARTEXT ) != 87 ) {
                        player.x--;
                        wattron( win, COLOR_PAIR( PLAYER ) );
                        wprintw( win, "G" );
                        wattroff( win, COLOR_PAIR( PLAYER ) );

                        wmove( win, player.y, player.x + 1 );

                        wattron( win, COLOR_PAIR( FLOOR ) );
                        wprintw( win, "F" );
                        wattroff( win, COLOR_PAIR( FLOOR ) );
                    }
                    break;
                default:
                    break;
            }
        } // kbhit

        wmove( twin, 1, 1 );
        wprintw( twin, "%d %d", player.x, player.y );
        wrefresh( win );
        wrefresh( twin );
        refresh();
    }
}

void print_map( char filename[], Player *player, WINDOW *win, WINDOW *twin ) {
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
                    wprintw( win, "#" );
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
                    yg = i;
                    xg = j;
                    break;
                case FLOOR:
                    wattron( win, COLOR_PAIR( FLOOR ) );
                    wprintw( win, "." );
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
    player -> x = xg;
    player -> y = yg;
    fclose( file );
}
