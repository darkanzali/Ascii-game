#include <ncursesw/ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "struktury.h"
#include "game.h"
#include "okna.h"

#define PLAYER_ATK 2

void print_map( int map, Player *player, Monster *monsters, Monster_list **fmonster_on_map, WINDOW *win, WINDOW *twin ); // Funkcja do wypisania całej mapy
void prplayer_xy( WINDOW *win, int y, int x ); // Funkcja wypisująca gracza na mapie
void prfch_xy( WINDOW *win, char ch, int y, int x ); // Funkcja wypisująca znak na mapie
void prmon_xy( WINDOW *win, Monster *monsters, int id, int y, int x ); // Funkcja wypisująca potwora na mapie
void prlife( WINDOW *win, Player player, Monster_list *monster );
void load_monsters( Monster **monsters, WINDOW *win ); // Funkcja wczytująca potwory z pliku do tablicy
void add_monster( Monster *monsters, Monster_list **fmonster, int id, int uniId, int y, int x ); // Funkcja dodająca potwora na mapę
void delete_dead_monster( WINDOW *win, Monster_list **pointer, Monster_list **fmonster ); // Funkcja usuwająca potwora z listy
void clear_list( Monster_list **monster ); // Funkcja czyszcząca listę
Monster_list *checkIfMonsterNearPlayer( Player *player, Monster_list *fmonster ); // Funkcja sprawdzająca czy potwór jest obok gracza
int time_diff( struct timeval start, struct timeval end );
int more_random( long max );
void save_game( WINDOW *win, int world );
void print_list( Monster_list *fmonster, WINDOW *win ); //DEBUG
int playGame( int world, Windows w ) {
    WINDOW *win = w.mwin;
    WINDOW *twin = w.twin;
    WINDOW *rwin = w.rwin;
    printBorder( rwin );
    //WINDOW *listawin = newwin( 10, 30, 0, 0 ); //DEBUG

    Player player; // Zmienna przechowująca dane o graczu
    Monster *monsters; // Wskaźnik na tablicę w której będą zapisane wszystkie potwory
    Monster_list *fmonster_on_map, *pointer; // Wskaźnik na pierwszy element listy potworów na mapie
    fmonster_on_map = NULL; // Ustawiamy wskaźnik na nic
    pointer = NULL; // Ustawiamy wskaźnik na nic
    bool goMenu = false; // Zmienna przechowująca dane czy chcemy wyjść do menu i ustawiamy że nie chcemy
    struct timeval mStartMove, mStartAtk, pStartAtk, pStartReg, EndTime;
    /* czas od ostatniego ruchu potworów, czas od ostatniego ataku potwora, czas od ostatniego ataku gracza */
    /* czas od ostatniej regeneracji życia gracza, obecny czas */
    unsigned long long seconds, useconds, breakTime;
    /* wyliczone sekundy, wyliczone mikrosekundy, czas przerwy */
    int mlife = 0; //DEBUG
    int atkStren; // Zmienna na siłę ataku
    int c; // Zmienna na wczytany znak z klawiatury
    int nworld = -1;

    wclear( win ); // Czyścimy główne okno
    printBorder( win ); // Wypisujemy ramkę głównego okna
    init_colors(); // Inicjalizujemy kolory
    load_monsters( &monsters, twin ); // Wczytujemy potwory z pliku z potworami
    wmove( twin, 1, 1 ); // Przemieszczamy karetkę w lewy górny róg ekranu
    print_map( world, &player, monsters, &fmonster_on_map, win, twin ); // Wypisujemy mapę
    wrefresh( win ); // Odświeżamy okno z mapą żeby się wyświetliła
    gettimeofday( &mStartMove, NULL ); // Wczytujemy na początek czasy
    gettimeofday( &mStartAtk, NULL );  // potrzebne do wyliczania
    gettimeofday( &pStartAtk, NULL );  // w grze czy mają nastąpić
    gettimeofday( &pStartReg, NULL );  // odpowiednie wydarzenia
    srand( time( NULL ) ); // Inicjalizujemy generator liczb pseudolosowych

    prlife( rwin, player, NULL );

    while( !goMenu ) {
        gettimeofday( &EndTime, NULL ); // Na sam początek pobieramy aktualny czas

        // Sprawdzamy czy gracz jest w trakcie walki
        player.attacking = checkIfMonsterNearPlayer( &player, fmonster_on_map ); // Wczytujemy wskaźnik na potwora ktory stoi obok gracza
        if( player.attacking != NULL ) // Jeżeli wskaźnik nie jest pusty to gracz jest w trakcie walki
            player.war = 1; // Ustawiamy status gracza na "w trakcie walki"
        else
            player.war = 0; // Jeżeli wskaźnik jest pusty to gracz z nikim nie walczy
        // Kończymy sprawdzanie czy gracz jest w trakcie walki

        // Sprawdzamy czy jakiś znak czeka na wczytanie
        if( kbhit() ) {
            c = getch(); // Wczytujemy ten znak
            switch( c ) { // Sprawdzamy co gracz chciał zrobić
                case 'a': // Atakujemy
                    breakTime = time_diff( pStartAtk, EndTime ); // Liczymy ile czasu minęło od ostatniego ataku
                    if( breakTime > 400 ) { // Jeżeli od ostatniej walki minęło 0,4 sekundy to możemy atakować
                        int atakuja = 0; //DEBUG

                        if( player.attacking != NULL ) {    // Upewniamy się czy obok gracza stoi potwór
                            atkStren = 0;                   // Zerujemy siłę ataku
                            atkStren = more_random( player.atk ); // Losujemy bonus do ataku z przedziału <0;siła_gracza>
                            atkStren += player.atk;         // Do siły ataku dodajemy siłę gracza
                            // TODO dodać broń
                            player.attacking -> hp = player.attacking -> hp - atkStren; // Pomniejszamy życie potwora o zadany cios
                            if( player.attacking -> hp < 0 ) player.attacking -> hp = 0; // Dla uniknięcia problemów jeżeli życie potwora jest mniejsze od zera to je zerujemy
                            atakuja = 1; //DEBUG
                            gettimeofday( &pStartAtk, NULL ); // Ustalamy czas ostatniego ataku
                            if( player.attacking -> hp == 0 ) {
                                delete_dead_monster( win, &player.attacking, &fmonster_on_map );
                                player.war = 0;
                                player.attacking = NULL;
                                //print_list( fmonster_on_map, listawin );
                            }
                        }

                        if( player.attacking == NULL || player.attacking -> hp == 0 ) //DEBUG
                            mlife = 0; //DEBUG
                        wmove( twin, 1, 15 ); //DEBUG
                        if(atakuja) //DEBUG
                            wprintw( twin, "atakujem                        " ); //DEBUG
                        else //DEBUG
                            wprintw( twin, "nie atakujem                    " ); //DEBUG
                    } else { // Jak nie minęło 0,4 sekundy to musimy odpocząć
                        wmove( twin, 1, 15 );
                        wprintw( twin, "zwolnij kowboju, musisz odpocząć" );
                    } // Koniec atakowania lub odpoczynku
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
                        if( fieldToGo == TELEPORT_CH )
                            return world + 1;
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
                        if( fieldToGo == TELEPORT_CH )
                            return world + 1;
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
                        if( fieldToGo == TELEPORT_CH )
                            return world + 1;
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
                        if( fieldToGo == TELEPORT_CH ) {
                            nworld = world + 1;
                            goMenu = true;
                        }
                    }
                    break;
                default:
                    break;
            }
        } // kbhit

        // Jeszcze raz sprawdzamy czy gracz jest w trakcie walki
        player.attacking = checkIfMonsterNearPlayer( &player, fmonster_on_map ); // Wczytujemy wskaźnik na potwora ktory stoi obok gracza
        if( player.attacking != NULL ) { // Jeżeli wskaźnik nie jest pusty to gracz jest w trakcie walki
            player.war = 1; // Ustawiamy status gracza na "w trakcie walki"
            gettimeofday( &pStartReg, NULL );
        } else
            player.war = 0; // Jeżeli wskaźnik jest pusty to gracz z nikim nie walczy
        // Kończymy sprawdzanie czy gracz jest w trakcie walki

        // Jeżeli gracz nie jest w trakcie walki i ma poniżej połowy życia to regenerujemy życie
        if( ( player.war == 0 ) && ( player.hp < player.maxhp / 2 ) ) {
            breakTime = time_diff( pStartReg, EndTime ); // Liczymy ile czasu zostało od ostatniej regeneracji
            if( breakTime > 1000 ) { // Jeżeli od ostatniej regeneracji minęła sekunda to regenerujemy
                if( more_random( 2 ) == 2 ) // Gracz ma ~33% szans na to że zregeneruje mu się życie w tej sekundzie
                    player.hp++;
                if( player.hp > player.maxhp ) player.hp = player.maxhp / 2;
                gettimeofday( &pStartReg, NULL );
            }
        }
        // Kończymy regenerowanie życia

        // Jeżeli gracz jest w trakcie walki to szukamy potwora z którym walczy
        if( player.war == 1 ) {
            wmove( twin, 1, 60 ); //DEBUG
            //wprintw( twin, "%p", player.attacking ); //DEBUG
            if( player.attacking != NULL ) { // Upewniamy się że znaleźliśmy potwora
                player.attacking -> war = 1; // Ustawiamy status potwora na walczącego
                breakTime = time_diff( mStartAtk, EndTime ); // Liczymy ile czasu minęło od ostatniego ataku potwora
                if( breakTime > 700 ) { // Potwór atakuje. Damy fory graczowi, gra niekoniecznie ma być trudna ;)
                    int atkStren;
                    atkStren = more_random( player.attacking -> atk );
                    if( atkStren != 0 )
                        atkStren += player.attacking -> atk;
                    player.hp -= atkStren;
                    if( player.hp <= 0 ) {
                        player.hp = 0;
                        prlife( rwin, player, player.attacking );
                        wclear( twin );
                        printBorder( twin );
                        wmove( twin, 1, 1 );
                        wprintw( twin, "Zginąłeś :( zaczynamy poziom od nowa? (t/n)" );
                        wrefresh( twin );
                        int out;
                        do {
                            out = getch();
                        } while( out != 't' && out != 'n' );
                        if( out == 't' ) {
                            goMenu = true;
                            nworld = world;
                        } else {
                            goMenu = true;
                            nworld = -1;
                        }
                    }
                    gettimeofday( &mStartAtk, NULL );
                } // Potwór kończy atakować
                mlife = player.attacking -> hp; //DEBUG
            }
        }
        // Kończymy walkę potwora z graczem

        breakTime = time_diff( mStartMove, EndTime ); // Liczymy czas od ostatniego ruchu potwora
        if( breakTime >= 1000 ) { // Jeżeli minęła sekunda potwory się ruszają
            pointer = fmonster_on_map;
            while( pointer != NULL ) {
                if( pointer -> war == 0 ) {
                    int way = more_random( 3 );
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

        prlife( rwin, player, player.attacking );
        wrefresh( win );
        wrefresh( twin );
        usleep( 500 ); // zatrzymujemy grę na 50 milisekund żeby nie obciążać procesora
    }
    free( monsters ); // Jak wychodzimy do menu to uwalniamy pamięć zajętą przez tablicę z potworami
    while( fmonster_on_map != NULL ) {
        clear_list( &fmonster_on_map );
    }

    return nworld;
}

void print_map( int map, Player *player, Monster *monsters, Monster_list **fmonster_on_map, WINDOW *win, WINDOW *twin ) {
    FILE *file;
    char id[ 3 ] = { 0, 0, 0 };
    char mapfile[ MAX_MAP_FILENAME ];
    char fileExt[ 5 ] = { '.', 'b', 'i', 'n', 0 };

    id[ 1 ] = 0;
    id[ 2 ] = 0;

    if( map > 9 ) {
        id[ 1 ] = ( map % 10 ) + '0';
        map /= 10;
    }
    id[ 0 ] = map + '0';

    strcpy( mapfile, "files/" );
    strcat( mapfile, id );
    strcat( mapfile, fileExt );

    file = fopen( mapfile, "rb" );
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
                    player -> hp = 20;
                    player -> maxhp = 20;
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

void prlife( WINDOW *win, Player player, Monster_list *monster ) {
    if( true ) {
        wmove( win, 0, 3 );
        wprintw( win, "Życie" );
        wmove( win, 2, 1 );
        wprintw( win, "Twoje" );
    }
    int d1, d2, d3;
    int perc, colored;
    int i;
    double h = player.hp;
    double mh = player.maxhp;

    perc = (double) ( ( h / mh ) * 100 );
    wmove( win, 3, 1 );
    colored = perc / 10;

    d3 = perc % 10;
    d2 = ( perc / 10 ) % 10;
    d1 = ( perc / 100 ) % 10;

    won( win, GREEN );
    for( i = 1; i <= colored; i++ ) {
        if( i == 4 || i == 5 || i == 6 || i == 7 ) {
            if( i == 4 && d1 != 0 ) {
                wprintw( win, "%d", d1 );
            } else if( i == 5 && ( ( d1 == 1 ) || ( d1 == 0 && d2 != 0 ) ) ) {
                wprintw( win, "%d", d2 );
            } else if( i == 6 ) {
                wprintw( win, "%d", d3 );
            } else if( i == 7 ) {
                wprintw( win, "%c", 37 );
            } else {
                wprintw( win, "." );
            }
        } else {
            wprintw( win, "." );
        }
    }
    woff( win, GREEN );

    won( win, RED );
    for( ; i <= 10; i++ ) {
        if( i == 4 || i == 5 || i == 6 || i == 7 ) {
            if( i == 4 && d1 != 0 ) {
                wprintw( win, "%d", d1 );
            } else if( i == 5 && ( ( d1 == 1 ) || ( d1 == 0 && d2 != 0 ) ) ) {
                wprintw( win, "%d", d2 );
            } else if( i == 6 ) {
                wprintw( win, "%d", d3 );
            } else if( i == 7 ) {
                wprintw( win, "%%" );
            } else {
                wprintw( win, "." );
            }
        } else {
            wprintw( win, "." );
        }
    }
    woff( win, RED );

    //5 1
    // Życie potwora
    wmove( win, 5, 1 );
    wprintw( win, "          " );
    wmove( win, 6, 1 );
    wprintw( win, "          " );
    wmove( win, 5, 1 );

    if( monster != NULL ) {
        h = monster -> hp;
        mh = monster -> maxhp;

        perc = (double) ( ( h / mh ) * 100 );
        colored = perc / 10;

        d3 = perc % 10;
        d2 = ( perc / 10 ) % 10;
        d1 = ( perc / 100 ) % 10;

        wprintw( win, "%s", monster -> name );
        wmove( win, 6, 1 );
        won( win, GREEN );
        for( i = 1; i <= colored; i++ ) {
            if( i == 4 || i == 5 || i == 6 || i == 7 ) {
                if( i == 4 && d1 != 0 ) {
                    wprintw( win, "%d", d1 );
                } else if( i == 5 && ( ( d1 == 1 ) || ( d1 == 0 && d2 != 0 ) ) ) {
                    wprintw( win, "%d", d2 );
                } else if( i == 6 ) {
                    wprintw( win, "%d", d3 );
                } else if( i == 7 ) {
                    wprintw( win, "%c", 37 );
                } else {
                    wprintw( win, "." );
                }
            } else {
                wprintw( win, "." );
            }
        }
        woff( win, GREEN );

        won( win, RED );
        for( ; i <= 10; i++ ) {
            if( i == 4 || i == 5 || i == 6 || i == 7 ) {
                if( i == 4 && d1 != 0 ) {
                    wprintw( win, "%d", d1 );
                } else if( i == 5 && ( ( d1 == 1 ) || ( d1 == 0 && d2 != 0 ) ) ) {
                    wprintw( win, "%d", d2 );
                } else if( i == 6 ) {
                    wprintw( win, "%d", d3 );
                } else if( i == 7 ) {
                    wprintw( win, "%%" );
                } else {
                    wprintw( win, "." );
                }
            } else {
                wprintw( win, "." );
            }
        }
        woff( win, RED );
    }
    wrefresh( win );
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

void add_monster( Monster *monsters, Monster_list **fmonster, int id, int uniId, int y, int x ) {
    Monster_list *wsk, *new;

    if( *fmonster == NULL ) {
        ( *fmonster ) = malloc( sizeof( Monster_list ) );
        ( *fmonster ) -> prev = NULL;
        ( *fmonster ) -> next = NULL;
        new = *fmonster;
    } else {
        wsk = *fmonster;
        while( wsk -> next != NULL )
            wsk = wsk -> next;

        new = malloc( sizeof( Monster_list ) );
        new -> next = NULL;
        new -> prev = wsk;
        wsk -> next = new;
    }

    strcpy( new -> name, monsters[ id ].name );
    new -> id       = id;
    new -> uniId    = uniId;
    new -> letter   = monsters[ id ].letter;
    new -> hp       = monsters[ id ].hp;
    new -> maxhp    = monsters[ id ].hp;
    new -> weapon   = monsters[ id ].weapon;
    new -> armor    = monsters[ id ].armor;
    new -> fieldch  = monsters[ id ].fieldch;
    new -> war      = 0;
    new -> x        = x;
    new -> y        = y;
    new -> atk      = monsters[ id ].atk;
}

void delete_dead_monster( WINDOW *win, Monster_list **pointer, Monster_list **fmonster ) {
    int x, y, fieldch;
    x = ( *pointer ) -> x;
    y = ( *pointer ) -> y;
    fieldch = ( *pointer ) -> fieldch;

    if( *pointer == *fmonster ) {
        if( ( *pointer ) -> next != NULL ) {
            *fmonster = ( *fmonster ) -> next;
            ( *fmonster ) -> prev = NULL;
        } else {
            *fmonster = NULL;
        }
    } else {
        if( ( *pointer ) -> next == NULL ) {
            ( *pointer ) -> prev -> next = NULL;
        } else {
            ( *pointer ) -> prev -> next = ( *pointer ) -> next;
            ( *pointer ) -> next -> prev = ( *pointer ) -> prev;
        }
    }

    free( *pointer );

    wmove( win, y, x );
    wprintw( win, "%c", fieldch );
}

void clear_list( Monster_list **monster ) {
    Monster_list *Next;
    while( *monster != NULL ) {
        Next = (*monster) -> next;
        free( *monster );
        *monster = Next;
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

int time_diff( struct timeval start, struct timeval end ) {
    long difference, seconds, useconds;
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    difference = ( ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5 );
    return difference;
}

int more_random( long max ) {
    //struct timeval s, e;
    //gettimeofday( &s, NULL );
    unsigned long
    num_bins = ( unsigned long ) max + 1,
    num_rand = ( unsigned long ) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

    long x;
    do {
        x = random();
    } while( num_rand - defect <= ( unsigned long ) x );
    //gettimeofday( &e, NULL );
    //printw( "%d", x/bin_size );
    //refresh();

    return x / bin_size;
}

void save_game( WINDOW *win, int world ) {
    
}

void print_list( Monster_list *fmonster, WINDOW *win ) {
    Monster_list *wsk;
    wsk = fmonster;
    wclear( win );
    if(wsk == NULL ) wprintw( win, "pusty" );
    while( wsk != NULL ) {
        wprintw( win, "%p %p %p\n", wsk->prev, wsk, wsk -> next );
        wsk = wsk -> next;
    }
    wrefresh(win);
}
