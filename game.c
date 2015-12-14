#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>

#include "struktury.h"
#include "game.h"
#include "okna.h"

void print_map( char filename[], Player *player, Monster *monsters, Monster_list *fmonster_on_map, WINDOW *win, WINDOW *twin );
void prplayer_xy( WINDOW *win, int y, int x );
void prfch_xy( WINDOW *win, char ch, int y, int x );
void prmon_xy( WINDOW *win, Monster *monsters, int id, int y, int x );
void load_monsters( Monster **monsters, WINDOW *win );
void add_monster( Monster *monsters, Monster_list *fmonster, int id, int y, int x );
void print_list( Monster_list *fmonster );
int playGame( int world, WINDOW *win, WINDOW *twin ) {

    Player player;
    Monster *monsters;
    Monster_list *fmonster_on_map, *pointer;
    fmonster_on_map = malloc( sizeof( Monster_list ) );
    fmonster_on_map -> letter = 0;
    bool goMenu = false;
    struct timeval start, end;
    long mtime, seconds, useconds;

    wclear( win );
    printBorder( win );
    init_colors();
    load_monsters( &monsters, twin );
    wmove( twin, 1, 1 );
    print_map( MAPFILE, &player, monsters, fmonster_on_map, win, twin );
    wrefresh( win );
    refresh();
    gettimeofday( &start, NULL );
    srand( time( NULL ) );

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
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y + 1, player.x );
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
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y - 1, player.x );
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
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y, player.x - 1 );
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
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y, player.x + 1 );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                    }
                    break;
                default:
                    break;
            }
        } // kbhit
        gettimeofday( &end, NULL );
        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mtime = ( ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5 );
        if( mtime >= 1000  ) { // Jeżeli minęło 0.5 sekundy potwory się ruszają
            pointer = fmonster_on_map;
            while( pointer != NULL ) {
                int way = rand() % 4;
                switch( way ) {
                    case 0: // Up
                        if( true ) {
                            char fieldToGo = ( mvwinch( win, pointer -> y - 1, pointer -> x ) & A_CHARTEXT );
                            if( fieldToGo == FLOOR_CH ) {
                                pointer -> y--;
                                chtype sav = mvwinch( win, pointer -> y, pointer -> x );
                                prmon_xy( win, monsters, pointer -> id, pointer -> y, pointer -> x );
                                prfch_xy( win, pointer -> fieldch, pointer -> y + 1, pointer -> x );
                                pointer -> fieldch = ( sav & A_CHARTEXT );
                            }
                        }
                        break;
                    case 1: // Down
                        if( true ) {
                            char fieldToGo = ( mvwinch( win, pointer -> y + 1, pointer -> x ) & A_CHARTEXT );
                            if( fieldToGo == FLOOR_CH ) {
                                pointer -> y++;
                                chtype sav = mvwinch( win, pointer -> y, pointer -> x );
                                prmon_xy( win, monsters, pointer -> id, pointer -> y, pointer -> x );
                                prfch_xy( win, pointer -> fieldch, pointer -> y - 1, pointer -> x );
                                pointer -> fieldch = ( sav & A_CHARTEXT );
                            }
                        }
                        break;
                    case 2: // Left
                        if( true ) {
                            char fieldToGo = ( mvwinch( win, pointer -> y, pointer -> x - 1 ) & A_CHARTEXT );
                            if( fieldToGo == FLOOR_CH ) {
                                pointer -> x--;
                                chtype sav = mvwinch( win, pointer -> y, pointer -> x );
                                prmon_xy( win, monsters, pointer -> id, pointer -> y, pointer -> x );
                                prfch_xy( win, pointer -> fieldch, pointer -> y, pointer -> x + 1 );
                                pointer -> fieldch = ( sav & A_CHARTEXT );
                            }
                        }
                        break;
                    case 3: // Right
                        if( true ) {
                            char fieldToGo = ( mvwinch( win, pointer -> y, pointer -> x + 1 ) & A_CHARTEXT );
                            if( fieldToGo == FLOOR_CH ) {
                                pointer -> x++;
                                chtype sav = mvwinch( win, pointer -> y, pointer -> x );
                                prmon_xy( win, monsters, pointer -> id, pointer -> y, pointer -> x );
                                prfch_xy( win, pointer -> fieldch, pointer -> y, pointer -> x - 1 );
                                pointer -> fieldch = ( sav & A_CHARTEXT );
                            }
                        }
                        break;
                }
                pointer = pointer -> next;
            }
            gettimeofday( &start, NULL );
        } // Potwory kończą się ruszać

        wmove( twin, 1, 1 );
        wprintw( twin, "%d %d %d", player.x, player.y, mtime );
        wrefresh( win );
        wrefresh( twin );
    }
    free( monsters );
    return 0;
}

void print_map( char filename[], Player *player, Monster *monsters, Monster_list *fmonster_on_map, WINDOW *win, WINDOW *twin ) {
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
                    won( win, WALL );
                    wprintw( win, "#" );
                    woff( win, WALL );
                    break;
                case STOP_MONSTER:
                    won( win, FLOOR );
                    wprintw( win, "\"" );
                    woff( win, FLOOR );
                    break;
                case PLAYER:
                    won( win, PLAYER );
                    wprintw( win, "%c", PLAYER_CH );
                    woff( win, PLAYER );
                    player -> x = j;
                    player -> y = i;
                    player -> fieldch = '.';
                    break;
                case FLOOR:
                    won( win, FLOOR );
                    wprintw( win, "." );
                    woff( win, FLOOR );
                    break;
                case MONSTER:
                    won( win, MONSTER );
                    wprintw( win, "%c", monsters[ m.id - 1 ].letter );
                    add_monster( monsters, fmonster_on_map, m.id - 1, i, j );
                    woff( win, MONSTER );
                    break;
                case TELEPORT:
                    won( win, TELEPORT );
                    wprintw( win, "T" );
                    woff( win, TELEPORT );
                    break;
                case BOX:
                    won( win, BOX );
                    wprintw( win, "Q" );
                    woff( win, BOX );
                    break;
                default:
                    wprintw( win, "E" );
                    break;
            }
        }
    }
    fclose( file );
}

void prplayer_xy( WINDOW *win, int y, int x ) {
    won( win, PLAYER );
    wmove( win, y, x );
    wprintw( win, "%c", PLAYER_CH );
    woff( win, PLAYER );
}

void prfch_xy( WINDOW *win, char ch, int y, int x ) {
    wmove( win, y, x );
    wprintw( win, "%c", ch );
}

void prmon_xy( WINDOW *win, Monster *monsters, int id, int y, int x ) {
    won( win, MONSTER );
    wmove( win, y, x );
    wprintw( win, "%c", monsters[ id ].letter );
    woff( win, MONSTER );
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

void add_monster( Monster *monsters, Monster_list *fmonster, int id, int y, int x ) {
    Monster_list *wsk, *new;
    if( fmonster -> letter == 0 ) {
        fmonster -> next = NULL;
        new = fmonster;
    } else {
        wsk = fmonster;

        while( wsk -> next != NULL )
            wsk = wsk -> next;

        new = malloc( sizeof( Monster_list ) );
        new -> next = NULL;
        wsk -> next = new;
        new -> prev = wsk;
    }

    new -> id = id;
    new -> letter = monsters[ id ].letter;
    strcpy( new -> name, monsters[ id ].name );
    new -> hp = monsters[ id ].hp;
    new -> weapon = monsters[ id ].weapon;
    new -> armor = monsters[ id ].armor;
    new -> fieldch = monsters[ id ].fieldch;
    new -> x = x;
    new -> y = y;
}

void print_list( Monster_list *fmonster ) {
    Monster_list *wsk;
    wsk = fmonster;
    if(wsk == NULL ) printw( "NULL" );
    while( wsk != NULL ) {
        printw( "%c\n", wsk->letter );
        printw( "%s\n", wsk->name );
        printw( "%d\n", wsk->hp );
        printw( "%d\n", wsk->weapon );
        printw( "%d\n", wsk->armor );
        printw( "%c\n", wsk->fieldch );
        printw( "%d\n", wsk->x );
        printw( "%d\n", wsk->y );
        wsk = wsk -> next;
    }
    refresh();
    getchar();
}
