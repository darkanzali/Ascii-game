#include <ncursesw/ncurses.h>
#include <stdlib.h>

#include "struktury.h"
#include "game.h"
#include "okna.h"

void print_map( char filename[], Player *player, Monster *monsters, WINDOW *win, WINDOW *twin );
void prch_xy( WINDOW *win, int type, int id, int y, int x );
void load_monsters( Monster **monsters, WINDOW *win );
int playGame( int world, WINDOW *win, WINDOW *twin ) {

    Player player;
    Monster *monsters;
    bool goMenu = false;
    wclear( win );
    printBorder( win );
    init_colors();

    load_monsters( &monsters, twin );
    wmove( twin, 1, 1 );
    print_map( MAPFILE, &player, monsters, win, twin );
    wrefresh( win );
    refresh();

    int c;
    while( !goMenu ) {
        if( kbhit() ) {
            c = getch();
            switch( c ) {
                case 'q':
                    if( true ) {
                        char save_txt[ 3 ][ 80 ];
                        int i, j;
                        for( i = 1; i < 3; i++ )
                            for( j = 1; j < 79; j++ )
                                save_txt[ i ][ j ] = ( mvwinch( twin, i, j ) & A_CHARTEXT );
                        wclear( twin );
                        printBorder( twin );
                        printBorder( win );
                        wmove( twin, 1, 1 );
                        wprintw( twin, "Czy chcesz wyjść do menu? (t/n)" );
                        wrefresh( twin );
                        wrefresh( win );
                        do {
                            c = getch();
                        } while( c != 't' && c != 'n' );
                        if( c == 't' )
                            goMenu = true;
                        else
                            for( i = 1; i < 3; i++ ) {
                                wmove( twin, i, 1 );
                                for( j = 1; j < 79; j++ )
                                    wprintw( twin, "%c", save_txt[ i ][ j ] );
                            }
                    }
                    break;
                case KEY_UP: // Up
                    if( true ) {
                        char fieldToGo = ( mvwinch( win, player.y - 1, player.x ) & A_CHARTEXT );
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.y--;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prch_xy( win, PLAYER, ZERO, player.y, player.x );
                            prch_xy( win, ZERO, player.fieldch, player.y + 1, player.x );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                    }
                    break;
                case KEY_DOWN: // Down
                    if( true ) {
                        char fieldToGo = ( mvwinch( win, player.y + 1, player.x ) & A_CHARTEXT );
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.y++;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prch_xy( win, PLAYER, ZERO, player.y, player.x );
                            prch_xy( win, ZERO, player.fieldch, player.y - 1, player.x );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                    }
                    break;
                case KEY_RIGHT: // Right
                    if( true ) {
                        char fieldToGo = ( mvwinch( win, player.y, player.x + 1 ) & A_CHARTEXT );
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.x++;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prch_xy( win, PLAYER, ZERO, player.y, player.x );
                            prch_xy( win, ZERO, player.fieldch, player.y, player.x - 1 );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                    }
                    break;
                case KEY_LEFT: // Left
                    if( true ) {
                        char fieldToGo = ( mvwinch( win, player.y, player.x - 1 ) & A_CHARTEXT );
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.x--;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prch_xy( win, PLAYER, ZERO, player.y, player.x );
                            prch_xy( win, ZERO, player.fieldch, player.y, player.x + 1 );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
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
    free( monsters );
    return 0;
}

void print_map( char filename[], Player *player, Monster *monsters, WINDOW *win, WINDOW *twin ) {
    FILE *file;
    file = fopen( filename, "rb" );
    int hei, wid;
    fread( &hei, sizeof( int ), 1, file );
    fread( &wid, sizeof( int ), 1, file );

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
                    wprintw( win, "\"" );
                    wattroff( win, COLOR_PAIR( FLOOR ) );
                    break;
                case PLAYER:
                    wattron( win, COLOR_PAIR( PLAYER ) );
                    wprintw( win, "%c", PLAYER_CH );
                    wattroff( win, COLOR_PAIR( PLAYER ) );
                    player -> x = j;
                    player -> y = i;
                    player -> fieldch = '.';
                    break;
                case FLOOR:
                    wattron( win, COLOR_PAIR( FLOOR ) );
                    wprintw( win, "." );
                    wattroff( win, COLOR_PAIR( FLOOR ) );
                    break;
                case MONSTER:
                    wattron( win, COLOR_PAIR( FLOOR ) );
                    wprintw( win, "%c", monsters[ m.id - 1 ].letter );
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
    fclose( file );
}

void prch_xy( WINDOW *win, int type, int id, int y, int x ) {
    wmove( win, y, x );
    if( type == ZERO ) {
        if( id == '.' ) type = FLOOR;
        if( id == '\"' ) type = STOP_MONSTER;
    }
    switch( type ) {
        case PLAYER:
            wattron( win, COLOR_PAIR( PLAYER ) );
            wprintw( win, "%c", PLAYER_CH );
            wattroff( win, COLOR_PAIR( PLAYER ) );
            break;
        case WALL:
            wattron( win, COLOR_PAIR( WALL ) );
            wprintw( win, "%c", WALL_CH );
            wattroff( win, COLOR_PAIR( WALL ) );
            break;
        case STOP_MONSTER:
            wattron( win, COLOR_PAIR( FLOOR ) );
            wprintw( win, "%c", STOP_MONSTER_CH );
            wattroff( win, COLOR_PAIR( FLOOR ) );
            break;
        case FLOOR:
            wattron( win, COLOR_PAIR( FLOOR ) );
            wprintw( win, "%c", FLOOR_CH );
            wattroff( win, COLOR_PAIR( FLOOR ) );
            break;
        case MONSTER:
            wattron( win, COLOR_PAIR( FLOOR ) );
            wprintw( win, " " );
            wattroff( win, COLOR_PAIR( FLOOR ) );
            break;
        case TELEPORT:
            wattron( win, COLOR_PAIR( TELEPORT ) );
            wprintw( win, "%c", TELEPORT_CH );
            wattroff( win, COLOR_PAIR( TELEPORT ) );
            break;
        case BOX:
            wattron( win, COLOR_PAIR( BOX ) );
            wprintw( win, "%c", BOX_CH );
            wattroff( win, COLOR_PAIR( BOX ) );
            break;
        default:
            wprintw( win, "E" );
            break;
    }
}

void load_monsters( Monster **monsters, WINDOW *win ) {
    FILE *file;
    file = fopen( MONSTERSFILE, "rb" );
    int count;
    fread( &count, sizeof( count ), 1, file );
    *monsters = malloc( count * sizeof( Monster ) );
    fread( *monsters, sizeof( Monster ), count, file );
    fclose( file );
}
