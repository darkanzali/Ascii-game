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

#define PLAYER_ATK 1

void print_map( char filename[], Player *player, Monster *monsters, Monster_list *fmonster_on_map, WINDOW *win, WINDOW *twin );
void prplayer_xy( WINDOW *win, int y, int x );
void prfch_xy( WINDOW *win, char ch, int y, int x );
void prmon_xy( WINDOW *win, Monster *monsters, int id, int y, int x );
void load_monsters( Monster **monsters, WINDOW *win );
void add_monster( Monster *monsters, Monster_list *fmonster, int id, int uniId, int y, int x );
void check_monsters_life( WINDOW *win, Monster_list *fmonster );
Monster_list *checkIfMonsterNearPlayer( Player *player, Monster_list *fmonster );
Monster_list *checkIfPlayerNearMonster( Player *player, Monster_list *fmonster );
void print_list( Monster_list *fmonster );
int playGame( int world, WINDOW *win, WINDOW *twin ) {

    Player player;
    Monster *monsters;
    Monster_list *fmonster_on_map, *pointer;
    fmonster_on_map = malloc( sizeof( Monster_list ) );
    fmonster_on_map -> letter = 0;
    bool goMenu = false;
    struct timeval mStartMove, mEndMove, pStartAtk, pEndAtk, mStartAtk, mEndAtk;
    long mtime, seconds, useconds, breakTime;
    Monster_list *wsk = NULL;

    wclear( win );
    printBorder( win );
    init_colors();
    load_monsters( &monsters, twin );
    wmove( twin, 1, 1 );
    print_map( MAPFILE, &player, monsters, fmonster_on_map, win, twin );
    wrefresh( win );
    refresh();
    gettimeofday( &mStartMove, NULL );
    gettimeofday( &pStartAtk, NULL );
    gettimeofday( &mStartAtk, NULL );
    srand( time( NULL ) );

    int c;
    while( !goMenu ) {
        if( kbhit() ) {
            c = getch();
            switch( c ) {
                case 'a': // Atakujemy
                    gettimeofday( &pEndAtk, NULL );
                    seconds  = pEndAtk.tv_sec  - pStartAtk.tv_sec;
                    useconds = pEndAtk.tv_usec - pStartAtk.tv_usec;
                    breakTime = ( ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5 );
                    if( breakTime > 400 ) {
                        wsk = NULL;
                        int matk, marm, mweap, atakuja = 0;
                        wsk = checkIfMonsterNearPlayer( &player, fmonster_on_map );

                        if( wsk != NULL ) {
                            
                            atakuja = 1;
                        }

                        wmove( twin, 1, 15 );
                        if(atakuja)
                            wprintw( twin, "atakujem                        " );
                        else
                            wprintw( twin, "nie atakujem                    " ); // Kończymy atakować
                        gettimeofday( &pStartAtk, NULL );
                    } else {
                        wmove( twin, 1, 15 );
                        wprintw( twin, "zwolnij kowboju, musisz odpocząć" );
                    }
                    break;
                case 'q': // Wychodzimy
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
                    } // Kończymy wychodzić
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

        wsk = NULL;
        wsk = checkIfPlayerNearMonster( &player, fmonster_on_map );
        wmove( twin, 1, 40 );
        wprintw( twin, "%p", wsk );
        if( wsk != NULL ) {
            wsk -> war = 1;
            gettimeofday( &mEndAtk, NULL );
            seconds  = mEndAtk.tv_sec  - mStartAtk.tv_sec;
            useconds = mEndAtk.tv_usec - mStartAtk.tv_usec;
            breakTime = ( ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5 );
            if( breakTime > 700 ) { // Damy fory graczowi, gra niekoniecznie ma być trudna ;)

            } // Gracz kończy atakować
            wsk = NULL;
        }

        gettimeofday( &mEndMove, NULL );
        seconds  = mEndMove.tv_sec  - mStartMove.tv_sec;
        useconds = mEndMove.tv_usec - mStartMove.tv_usec;
        mtime = ( ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5 );
        if( mtime >= 1000 ) { // Jeżeli minęło 0.5 sekundy potwory się ruszają
            pointer = fmonster_on_map;
            while( pointer != NULL ) {
                if( pointer -> war == 0 ) {
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
                }
                pointer = pointer -> next;
            }
            gettimeofday( &mStartMove, NULL );
        } // Potwory kończą się ruszać
        check_monsters_life( win, fmonster_on_map );
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
    int mCount = 0;
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
                    wprintw( win, "%c", WALL_CH );
                    woff( win, WALL );
                    break;
                case STOP_MONSTER:
                    won( win, FLOOR );
                    wprintw( win, "%c", STOP_MONSTER_CH );
                    woff( win, FLOOR );
                    break;
                case PLAYER:
                    won( win, PLAYER );
                    wprintw( win, "%c", PLAYER_CH );
                    woff( win, PLAYER );
                    player -> x = j;
                    player -> y = i;
                    player -> fieldch = '.';
                    player -> atk = PLAYER_ATK;
                    break;
                case FLOOR:
                    won( win, FLOOR );
                    wprintw( win, "%c", FLOOR_CH );
                    woff( win, FLOOR );
                    break;
                case MONSTER:
                    won( win, MONSTER );
                    wprintw( win, "%c", monsters[ m.id - 1 ].letter );
                    mCount++;
                    add_monster( monsters, fmonster_on_map, m.id - 1, mCount, i, j );
                    woff( win, MONSTER );
                    break;
                case TELEPORT:
                    won( win, TELEPORT );
                    wprintw( win, "%c", TELEPORT_CH );
                    woff( win, TELEPORT );
                    break;
                case BOX:
                    won( win, BOX );
                    wprintw( win, "%c", BOX_CH );
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

void add_monster( Monster *monsters, Monster_list *fmonster, int id, int uniId, int y, int x ) {
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

    strcpy( new -> name, monsters[ id ].name );
    new -> id       = id;
    new -> uniId    = uniId;
    new -> letter   = monsters[ id ].letter;
    new -> hp       = monsters[ id ].hp;
    new -> weapon   = monsters[ id ].weapon;
    new -> armor    = monsters[ id ].armor;
    new -> fieldch  = monsters[ id ].fieldch;
    new -> war      = 0;
    new -> x        = x;
    new -> y        = y;
}

void check_monsters_life( WINDOW *win, Monster_list *fmonster ) {
    Monster_list *pointer, *prev, *next;
    int x, y, fieldch;
    pointer = fmonster;

    while( pointer != NULL ) {
        if( pointer -> hp == 0 ) {
            x = pointer -> x;
            y = pointer -> y;
            fieldch = pointer -> fieldch;
            if( pointer -> prev != NULL ) {
                prev = pointer -> prev;
                if( pointer -> next != NULL )
                    prev = pointer -> next;
                else
                    prev = NULL;
            } else {
                next = pointer -> next;
                next -> prev = NULL;
            }
            next = pointer -> next;
            free( pointer );
            pointer = next;
            wmove( win, y, x );
            wprintw( win, "%c", fieldch );
        }
        pointer = pointer -> next;
    }
}

Monster_list *checkIfMonsterNearPlayer( Player *player, Monster_list *fmonster ) {
    Monster_list *wsk;
    wsk = fmonster;
    int px = player -> x;
    int py = player -> y;
    int mx, my;
    int odlx, odly;
    while( wsk != NULL ) {
        mx = wsk -> x;
        my = wsk -> y;
        odlx = abs( px - mx );
        odly = abs( py - my );
        if( odlx + odly == 1 )
            return wsk;

        wsk -> war = 0;
        wsk = wsk -> next;
    }
    return NULL;
}

Monster_list *checkIfPlayerNearMonster( Player *player, Monster_list *fmonster ) {
    Monster_list *wsk;
    wsk = fmonster;
    int px = player -> x;
    int py = player -> y;
    int mx, my;
    int odlx, odly;
    while( wsk != NULL ) {
        mx = wsk -> x;
        my = wsk -> y;
        odlx = abs( px - mx );
        odly = abs( py - my );
        if( odlx + odly == 1 )
            return wsk;

        wsk -> war = 0;
        wsk = wsk -> next;
    }
    return NULL;
}

void print_list( Monster_list *fmonster ) {
    Monster_list *wsk;
    wsk = fmonster;
    if(wsk == NULL ) printw( "pusty" );
    while( wsk != NULL ) {
        printw( "%p %p %p\n", wsk->prev, wsk, wsk -> next );
        wsk = wsk -> next;
    }
    refresh();
    getchar();
}
