/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

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
#include "funkcje.h"

int playGame( int world, Windows w ) {
    WINDOW *win = w.mwin;
    WINDOW *twin = w.twin;
    WINDOW *rwin = w.rwin;
    WINDOW *ewin = w.ewin;
    printBorder( rwin );
    printBorder( ewin );

    Player player; // Zmienna przechowująca dane o graczu
    Monster *monsters; // Wskaźnik na tablicę w której będą zapisane wszystkie potwory
    Monster_list *fmonster_on_map, *pointer; // Wskaźnik na pierwszy element listy potworów na mapie
    Weapon *weapons;
    Armor *armors;
    Box_list *fbox = NULL;
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
    int max_worlds;

    wclear( win ); // Czyścimy główne okno
    printBorder( win ); // Wypisujemy ramkę głównego okna
    init_colors(); // Inicjalizujemy kolory
    load_monsters( &monsters ); // Wczytujemy potwory z pliku z potworami
    load_armors( &armors );
    load_weapons( &weapons );
    load_maxworlds( &max_worlds );
    wmove( twin, 1, 1 ); // Przemieszczamy karetkę w lewy górny róg ekranu
    load_player( &player );
    if( world != -1 ) {
        if( world == 1 );
            init_player( &player );
        print_map( world, &player, monsters, &fmonster_on_map, &fbox, win, twin ); // Wypisujemy mapę
    } else {
        int load = load_saved_game( &player, monsters, &fmonster_on_map, win );
        if( load == -1 ) {
            goMenu = true;
            nworld = -1;
        } else
            world = player.place;
    }
    wrefresh( win ); // Odświeżamy okno z mapą żeby się wyświetliła
    gettimeofday( &mStartMove, NULL ); // Wczytujemy na początek czasy
    gettimeofday( &mStartAtk, NULL );  // potrzebne do wyliczania
    gettimeofday( &pStartAtk, NULL );  // w grze czy mają nastąpić
    gettimeofday( &pStartReg, NULL );  // odpowiednie wydarzenia
    srand( time( NULL ) ); // Inicjalizujemy generator liczb pseudolosowych

    if( !goMenu ) {
        prlife( rwin, player, NULL );
        print_player_info( ewin, player, armors, weapons );
    }

    while( !goMenu ) {
        beginning:
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
                case 's':
                    save_game( win, world, player, fmonster_on_map, monsters );
                    break;
                case 'l':
                    load_saved_game( &player, monsters, &fmonster_on_map, win );
                    break;
                case 'a': // Atakujemy
                    breakTime = time_diff( pStartAtk, EndTime ); // Liczymy ile czasu minęło od ostatniego ataku
                    if( breakTime > 400 ) { // Jeżeli od ostatniej walki minęło 0,4 sekundy to możemy atakować
                        int atakuja = 0; //DEBUG

                        if( player.attacking != NULL ) {    // Upewniamy się czy obok gracza stoi potwór
                            atkStren = 0;                   // Zerujemy siłę ataku
                            atkStren = more_random( player.atk ); // Losujemy bonus do ataku z przedziału <0;siła_gracza>
                            atkStren += player.atk;         // Do siły ataku dodajemy siłę gracza
                            atkStren += more_random( weapons[ player.weapon ].atk );
                            player.attacking -> hp = player.attacking -> hp - atkStren; // Pomniejszamy życie potwora o zadany cios
                            if( player.attacking -> hp < 0 ) player.attacking -> hp = 0; // Dla uniknięcia problemów jeżeli życie potwora jest mniejsze od zera to je zerujemy
                            atakuja = 1; //DEBUG
                            gettimeofday( &pStartAtk, NULL ); // Ustalamy czas ostatniego ataku
                            if( player.attacking -> hp == 0 ) {
                                player.exp += player.attacking -> exp;
                                check_player_exp( &player );
                                print_player_info( ewin, player, armors, weapons );
                                wmove( twin, 1, 1 );
                                wprintw( twin, "%d ", player.attacking -> exp );
                                delete_dead_monster( win, &player.attacking, &fmonster_on_map );
                                player.war = 0;
                                player.attacking = NULL;
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
                        if( fieldToGo == BOX_CH ) {
                            open_box( &player, player.y - 1, player.x, &fbox, win );
                            print_player_info( ewin, player, armors, weapons );
                            prfch_xy( win, FLOOR_CH, player.y - 1, player.x );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == DOOR_CH && player.key == world ) {
                            prfch_xy( win, FLOOR_CH, player.y - 1, player.x );
                            player.key--;
                            print_player_info( ewin, player, armors, weapons );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.y--;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y + 1, player.x );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                        if( fieldToGo == TELEPORT_CH ) {
                            if( world == max_worlds )
                                nworld = -2;
                            else
                                nworld = world + 1;
                            goMenu = true;
                        }
                    }
                    break;
                case KEY_DOWN: // Down
                    if( true ) {
                        char fieldToGo = ( mvwinch( win, player.y + 1, player.x ) & A_CHARTEXT );
                        if( fieldToGo == BOX_CH ) {
                            open_box( &player, player.y + 1, player.x, &fbox, win );
                            print_player_info( ewin, player, armors, weapons );
                            prfch_xy( win, FLOOR_CH, player.y + 1, player.x );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == DOOR_CH && player.key == world ) {
                            prfch_xy( win, FLOOR_CH, player.y + 1, player.x );
                            player.key--;
                            print_player_info( ewin, player, armors, weapons );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.y++;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y - 1, player.x );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                        if( fieldToGo == TELEPORT_CH ) {
                            if( world == max_worlds )
                                nworld = -2;
                            else
                                nworld = world + 1;
                            goMenu = true;
                        }
                    }
                    break;
                case KEY_RIGHT: // Right
                    if( true ) {
                        char fieldToGo = ( mvwinch( win, player.y, player.x + 1 ) & A_CHARTEXT );
                        if( fieldToGo == BOX_CH ) {
                            open_box( &player, player.y, player.x + 1, &fbox, win );
                            print_player_info( ewin, player, armors, weapons );
                            prfch_xy( win, FLOOR_CH, player.y, player.x + 1 );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == DOOR_CH && player.key == world ) {
                            prfch_xy( win, FLOOR_CH, player.y, player.x + 1 );
                            player.key--;
                            print_player_info( ewin, player, armors, weapons );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.x++;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y, player.x - 1 );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                        if( fieldToGo == TELEPORT_CH ) {
                            if( world == max_worlds )
                                nworld = -2;
                            else
                                nworld = world + 1;
                            goMenu = true;
                        }
                    }
                    break;
                case KEY_LEFT: // Left
                    if( true ) {
                        char fieldToGo = ( mvwinch( win, player.y, player.x - 1 ) & A_CHARTEXT );
                        if( fieldToGo == BOX_CH ) {
                            open_box( &player, player.y, player.x - 1, &fbox, win );
                            print_player_info( ewin, player, armors, weapons );
                            prfch_xy( win, FLOOR_CH, player.y, player.x - 1 );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == DOOR_CH && player.key == world ) {
                            prfch_xy( win, FLOOR_CH, player.y, player.x - 1 );
                            player.key--;
                            print_player_info( ewin, player, armors, weapons );
                            fieldToGo = FLOOR_CH;
                        }
                        if( fieldToGo == FLOOR_CH || fieldToGo == STOP_MONSTER_CH ) {
                            player.x--;
                            chtype sav = mvwinch( win, player.y, player.x );
                            prplayer_xy( win, player.y, player.x );
                            prfch_xy( win, player.fieldch, player.y, player.x + 1 );
                            player.fieldch = ( sav & A_CHARTEXT );
                        }
                        if( fieldToGo == TELEPORT_CH ) {
                            if( world == max_worlds )
                                nworld = -2;
                            else
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
        if( ( player.war == 0 ) && ( player.hp < player.maxhp ) ) {
            breakTime = time_diff( pStartReg, EndTime ); // Liczymy ile czasu zostało od ostatniej regeneracji
            if( breakTime > 1000 ) { // Jeżeli od ostatniej regeneracji minęła sekunda to regenerujemy
                if( player.hp < player.maxhp / 2 ) {
                    if( more_random( 1 ) == 1 ) // Gracz ma 50% szans na to że zregeneruje mu się życie w tej sekundzie
                        player.hp++;
                    if( player.hp > player.maxhp / 2 ) player.hp = player.maxhp / 2;
                    gettimeofday( &pStartReg, NULL );
                } else {
                    if( more_random( 2 ) == 1 ) // Gracz ma ~33% szans na to że zregeneruje mu się życie w tej sekundzie
                    player.hp++;
                    if( player.hp > player.maxhp ) player.hp = player.maxhp;
                    gettimeofday( &pStartReg, NULL );
                }
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
                    atkStren -= more_random( armors[ player.armor ].def );
                    if( atkStren != 0 )
                        atkStren += player.attacking -> atk;
                    if( atkStren < 0 )
                        atkStren = 0;
                    player.hp -= atkStren;
                    if( player.hp <= 0 ) {
                        player.hp = 0;
                        prlife( rwin, player, player.attacking );
                        wclear( twin );
                        printBorder( twin );
                        wmove( twin, 1, 1 );
                        wprintw( twin, "Zginąłeś :( od (n)owa,(w)czytaj grę, (m)enu? (n/w/m)" );
                        wrefresh( twin );
                        int out;
                        do {
                            out = getch();
                        } while( out != 'n' && out != 'w' && out != 'm' );
                        wclear( twin );
                        printBorder( twin );
                        wrefresh( twin );
                        if( out == 'n' ) {
                            goMenu = true;
                            nworld = world;
                        } else if( out == 'w' ) {
                            load_saved_game( &player, monsters, &fmonster_on_map, win );
                            goMenu = false;
                            goto beginning;
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
    free( weapons );
    free( armors );
    while( fmonster_on_map != NULL ) {
        clear_list( &fmonster_on_map );
    }
    while( fbox != NULL ) {
        clear_list_box( &fbox );
    }

    return nworld;
}

void print_map( int map, Player *player, Monster *monsters, Monster_list **fmonster_on_map, Box_list **fbox, WINDOW *win, WINDOW *twin ) {
    FILE *file;
    char *mapfile;

    mapfile = world_to_char( "maps/", map, ".bin" );

    file = fopen( mapfile, "rb" );
    free( mapfile );
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
                    player -> place = map;
                    player -> x = j;
                    player -> y = i;
                    break;
                case FLOOR:
                    won( win, FLOOR );
                    wprintw( win, "%c", FLOOR_CH );
                    woff( win, FLOOR );
                    break;
                case DOOR:
                    won( win, TELEPORT );
                    wprintw( win, "%c", DOOR_CH );
                    woff( win, TELEPORT );
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
                    wprintw( win, " " );
                    break;
            }
        }
    }
    fclose( file );
    mapfile = world_to_char( "maps/", map, "i.bin" );
    file = fopen( mapfile, "rb" );
    free( mapfile );

    int x, y, boxes, objects;
    Item item;

    fread( &boxes, sizeof( int ), 1, file );

    for( i = 0; i < boxes; i++ ) {
        Item_list *fitem = NULL;
        fread( &y, sizeof( int ), 1, file );
        fread( &x, sizeof( int ), 1, file );
        fread( &objects, sizeof( int ), 1, file );
        for( j = 0; j < objects; j++ ) {
            fread( &item, sizeof( Item ), 1, file );
            add_to_item_list( &fitem, item );
        }
        add_box_to_list( fbox, y, x, fitem );
    }
    refresh();
    fclose( file );
}

void add_to_item_list( Item_list **fitem, Item item ) {
    Item_list *wsk, *new;
    if( *fitem == NULL ) {
        *fitem = malloc( sizeof( Item_list ) );
        new = *fitem;
        new -> next = NULL;
    } else {
        wsk = *fitem;
        while( wsk -> next != NULL )
            wsk = wsk -> next;
        new = malloc( sizeof( Item_list ) );
        wsk -> next = new;
        new -> next = NULL;
    }

    new -> type = item.type;
    new -> id   = item.id;
}

void add_box_to_list( Box_list **fbox, int y, int x, Item_list *fitem ) {
    Box_list *wsk, *new;
    if( *fbox == NULL ) {
        *fbox = malloc( sizeof( Box_list ) );
        new = *fbox;
        new -> next = NULL;
    } else {
        wsk = *fbox;
        while( wsk -> next != NULL )
            wsk = wsk -> next;
        new = malloc( sizeof( Box_list ) );
        new -> next = NULL;
        wsk -> next = new;
    }

    new -> x = x;
    new -> y = y;
    new -> fitem = fitem;
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
    int perc, colored;
    int i;
    double h = player.hp;
    double mh = player.maxhp;

    perc = (double) ( ( h / mh ) * 100 );
    wmove( win, 3, 1 );
    colored = perc / 10;

    char *life = l_to_char( player.hp, player.maxhp );

    won( win, GREEN );
    for( i = 1; i <= 10; i++ ) {
        if( i == colored + 1 ) {
            woff( win, GREEN );
            won( win, RED );
        }
        if( i - 1 <= strlen( life ) - 1 ) {
            wprintw( win, "%c", life[ i - 1 ] );
        } else {
            wprintw( win, "." );
        }
    }
    woff( win, RED );

    free( life );

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

        life = l_to_char( monster -> hp, monster -> maxhp );

        wprintw( win, "%s", monster -> name );
        wmove( win, 6, 1 );
        won( win, GREEN );
        for( i = 1; i <= 10; i++ ) {
            if( i == colored + 1 ) {
                woff( win, GREEN );
                won( win, RED );
            }
            if( i - 1 <= strlen( life ) - 1 ) {
                wprintw( win, "%c", life[ i - 1 ] );
            } else {
                wprintw( win, "." );
            }
        }
        woff( win, RED );
    }
    wrefresh( win );
}

void load_monsters( Monster **monsters ) {
    FILE *file;
    file = fopen( MONSTERSFILE, "rb" );
    int count;
    fread( &count, sizeof( int ), 1, file );
    *monsters = malloc( count * sizeof( Monster ) );
    fread( *monsters, sizeof( Monster ), count, file );
    fclose( file );
}

void load_weapons( Weapon **weapons ) {
    FILE *file;
    file = fopen( WEAPONSFILE, "rb" );
    int count;
    fread( &count, sizeof( int ), 1, file );
    *weapons = malloc( count * sizeof( Weapon ) );
    fread( *weapons, sizeof( Weapon ), count, file );
    fclose( file );
}

void load_armors( Armor **armors ) {
    FILE *file;
    file = fopen( ARMORSFILE, "rb" );
    int count;
    fread( &count, sizeof( int ), 1, file );
    *armors = malloc( count * sizeof( Armor ) );
    fread( *armors, sizeof( Armor ), count, file );
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
    new -> exp      = monsters[ id ].exp;
    new -> war      = 0;
    new -> x        = x;
    new -> y        = y;
    new -> atk      = monsters[ id ].atk;
}

void add_monster_saved( Monster_list mToAdd, Monster_list **fmonster, int uniId ) { // Funkcja dodająca potwora wczytanego z zapisu
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

    strcpy( new -> name, mToAdd.name );
    new -> id       = mToAdd.id;
    new -> uniId    = uniId;
    new -> letter   = mToAdd.letter;
    new -> hp       = mToAdd.hp;
    new -> maxhp    = mToAdd.maxhp;
    new -> weapon   = mToAdd.weapon;
    new -> armor    = mToAdd.armor;
    new -> fieldch  = mToAdd.fieldch;
    new -> exp      = mToAdd.exp;
    new -> war      = 0;
    new -> x        = mToAdd.x;
    new -> y        = mToAdd.y;
    new -> atk      = mToAdd.atk;
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
        Next = ( *monster ) -> next;
        free( *monster );
        *monster = Next;
    }
}

void clear_list_box( Box_list **fbox ) {
    Box_list *Next;
    Item_list *Fitem, *NextItem;
    while( *fbox != NULL ) {
        Fitem = ( *fbox ) -> fitem;
        while( Fitem != NULL ) {
            NextItem = Fitem -> next;
            free( Fitem );
            Fitem = NextItem;
        }

        Next = ( *fbox ) -> next;
        free( *fbox );
        *fbox = Next;
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

void save_game( WINDOW *win, int world, Player player, Monster_list *fmonster, Monster *monsters ) {
    if( access( "save/player.bin", F_OK ) != -1 ) { // Usuwanie pliku zapisu stanu gracza jeśli istnieje
        if( remove( "save/player.bin" ) != 0 ) {
            printf( "Error saving game (P). Exiting." );
            exit( 1 );
        }
    }

    char *map, *mons;

    map = world_to_char( "save/", world, ".bin" );
    mons = world_to_char( "save/", world, "m.bin" );

    if( access( map, F_OK ) != -1 ) { // Usuwanie pliku zapisu mapy jeśli istnieje
        if( remove( map ) != 0 ) {
            printf( "Error saving game (M). Exiting." );
            exit( 1 );
        }
    }

    if( access( mons, F_OK ) != -1 ) { // Usuwanie pliku zapisu listy potworów jeśli istnieje
        if( remove( mons ) != 0 ) {
            printf( "Error saving game (Mm). Exiting." );
            exit( 1 );
        }
    }

    FILE *plaf = fopen( "save/player.bin", "wb" );
    FILE *mapf = fopen( map, "wb" );
    FILE *monf = fopen( mons, "wb" );

    player.attacking = NULL;

    fwrite( &player, sizeof( Player ), 1, plaf );

    int i, j;
    char field;
    Map_field mfield;

    j = 80;
    i = 20;

    int x = j, y = i;

    fwrite( &i, sizeof( int ), 1, mapf );
    fwrite( &j, sizeof( int ), 1, mapf );
    for( i = 1; i <= y; i++ ) {
        for( j = 1; j <= x; j++ ) {
            field = ( mvwinch( win, i, j ) & A_CHARTEXT );
            mfield.type = ZERO;
            mfield.id   = ZERO;
            switch( field ) {
                case '#':
                    mfield.type = WALL;
                    mfield.id   = ZERO;
                    break;
                case '.':
                    mfield.type = FLOOR;
                    mfield.id   = ZERO;
                    break;
                case '\"':
                    mfield.type = STOP_MONSTER;
                    mfield.id   = ZERO;
                    break;
                case '@':
                    mfield.type = PLAYER;
                    mfield.id   = ZERO;
                    break;
                case 'T':
                    mfield.type = TELEPORT;
                    mfield.id   = ZERO;
                    break;
                case '&':
                    mfield.type = BOX;
                    mfield.id   = ZERO;
                    break;
                default: // Jak nierozpoznane to szukamy czy to nie potwór
                    if( true ) {
                        int im;
                        for( im = 0; monsters[ im ].hp != -1; im++ ) {
                            if( monsters[ im ].letter == field ) {
                                mfield.type = MONSTER;
                                mfield.id   = im + 1;
                                break;
                            }
                        }
                    }
                break;
            }
            fwrite( &mfield, sizeof( Map_field ), 1, mapf );
        }
    }

    Monster_list *wsk;

    wsk = fmonster;

    while( wsk != NULL ) {
        fwrite( wsk, sizeof( Monster_list ), 1, monf );
        wsk = wsk -> next;
    }

    fclose( plaf );
    fclose( mapf );
    fclose( monf );
    free( map );
    free( mons );
}

int load_player( Player *player ) {
    if( access( "save/player.bin", F_OK ) == -1 )
        return -1;

    FILE *plaf = fopen( "save/player.bin", "rb" );
    fread( player, sizeof( Player ), 1, plaf );
    fclose( plaf );
}

void init_player( Player *player ) {
    player -> hp = 30;
    player -> maxhp = 30;
    player -> fieldch = '.';
    player -> atk = 3;
    player -> def = 1;
    player -> weapon = ZERO;
    player -> armor = ZERO;
    player -> lvl = 1;
    player -> exp = 0;
    player -> key = 0;
}

int load_saved_game( Player *player, Monster *monsters, Monster_list **fmonster, WINDOW *win ) {
    int world;
    char *map, *mons;

    if( load_player( player ) == -1 )
        return -1;

    world = player -> place;

    map = world_to_char( "save/", world, ".bin" );
    mons = world_to_char( "save/", world, "m.bin" );

    if( access( map, F_OK ) == -1 ){
        free( map );
        free( mons );
        return -1;
    }
    if( access( mons, F_OK ) == -1 ) {
        free( map );
        free( mons );
        return -1;
    }

    while( *fmonster != NULL ) {
        clear_list( fmonster );
    }

    FILE *mapf = fopen( map, "rb" );
    FILE *monf = fopen( mons, "rb" );

    int x, y;
    fread( &y, sizeof( int ), 1, mapf );
    fread( &x, sizeof( int ), 1, mapf );

    wclear( win );
    printBorder( win );

    int i, j;
    Map_field m;
    int mCount = 0;
    Monster_list tMonster;

    for( i = 1; i <= y; i++ ) {
        wmove( win, i, 1 );
        for( j = 1; j <= x; j++ ) {
            fread( &m, sizeof( Map_field ), 1, mapf );
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
                    break;
                case FLOOR:
                    won( win, FLOOR );
                    wprintw( win, "%c", FLOOR_CH );
                    woff( win, FLOOR );
                    break;
                case DOOR:
                    won( win, TELEPORT );
                    wprintw( win, "%c", DOOR_CH );
                    woff( win, TELEPORT );
                    break;
                case MONSTER:
                    won( win, MONSTER );
                    wprintw( win, "%c", monsters[ m.id - 1 ].letter );
                    mCount++;
                    fread( &tMonster, sizeof( Monster_list ), 1, monf );
                    add_monster_saved( tMonster, fmonster, mCount );
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
                    wprintw( win, " " );
                    break;
            }
        }
    }

    free( map );
    free( mons );
}

void check_player_exp( Player *player ) {
    /*
    exp
    13 - lvl2
    30 - lvl3
    38 - lvl4
    60 - lvl5
    70 - lvl6
    81 - lvl7
    93 - lvl8
    112 - lvl9
    */
    if( player -> lvl < 2 && player -> exp >= 13 ) {
        player -> lvl = 2;
        player -> maxhp = 45;
        player -> atk = 6;
        player -> def = 2;
    }
    if( player -> lvl < 3 && player -> exp >= 30 ) {
        player -> lvl = 3;
        player -> maxhp = 60;
        player -> atk = 8;
        player -> def = 3;
    }
    if( player -> lvl < 4 && player -> exp >= 38 ) {
        player -> lvl = 4;
        player -> maxhp = 75;
        player -> atk = 11;
        player -> def = 4;
    }
    if( player -> lvl < 5 && player -> exp >= 60 ) {
        player -> lvl = 5;
        player -> maxhp = 90;
        player -> atk = 14;
        player -> def = 5;
    }
    if( player -> lvl < 6 && player -> exp >= 70 ) {
        player -> lvl = 6;
        player -> maxhp = 105;
        player -> atk = 17;
        player -> def = 6;
    }
    if( player -> lvl < 7 && player -> exp >= 81 ) {
        player -> lvl = 7;
        player -> maxhp = 120;
        player -> atk = 20;
        player -> def = 7;
    }
    if( player -> lvl < 8 && player -> exp >= 93 ) {
        player -> lvl = 8;
        player -> maxhp = 135;
        player -> atk = 23;
        player -> def = 8;
    }
    if( player -> lvl < 9 && player -> exp >= 112 ) {
        player -> lvl = 9;
        player -> maxhp = 250;
        player -> atk = 80;
        player -> def = 60;
    }
}

void print_player_info( WINDOW *win, Player player, Armor *armors, Weapon *weapons ) {
    wmove( win, 1, 1 );

    won( win, GREEN );
    wprintw( win, "Lvl:" );
    woff( win, GREEN );

    wprintw( win, " %d", player.lvl );
    wmove( win, 2, 1 );

    won( win, GREEN );
    wprintw( win, "Exp:" );
    woff( win, GREEN );
    wprintw( win, "      " );
    wmove( win, 2, 5 );

    wprintw( win, " %d", player.exp );
    wmove( win, 3, 1 );

    won( win, GREEN );
    wprintw( win, "Zbroja:" );
    woff( win, GREEN );

    wmove( win, 4, 1 );
    wprintw( win, "          ");
    wmove( win, 4, 1 );
    wprintw( win, "%s", armors[ player.armor ].name );
    wmove( win, 5, 1 );

    won( win, GREEN );
    wprintw( win, "Def:" );
    woff( win, GREEN );
    wprintw( win, " %d", armors[ player.armor ].def );

    wmove( win, 6, 1 );

    won( win, GREEN );
    wprintw( win, "Broń:" );
    woff( win, GREEN );

    wmove( win, 7, 1 );
    wprintw( win, "          ");
    wmove( win, 7, 1 );
    wprintw( win, "%s", weapons[ player.weapon ].name );

    wmove( win, 8, 1 );
    won( win, GREEN );
    wprintw( win, "Atk:" );
    woff( win, GREEN );
    wprintw( win, "      " );
    wmove( win, 8, 5 );
    wprintw( win, " %d", weapons[ player.weapon ].atk );
    wmove( win, 9, 1 );
    wprintw( win, "          " );
    wmove( win, 9, 1 );
    won( win, GREEN );
    wprintw( win, "Klucze:" );
    woff( win, GREEN );
    wprintw( win, " %d", player.key );

    wrefresh( win );
}

void open_box( Player *player, int y, int x, Box_list **fbox, WINDOW *win ) {
    Box_list *box = find_box( fbox, y, x );
    if( box == NULL ) // Nie udało się
        return;

    Item_list *fitem = box -> fitem, *Next;
    while( fitem != NULL ) {
        Next = fitem -> next;
        switch( fitem -> type ) {
            case ARMOR:
                player -> armor = fitem -> id;
                break;
            case WEAPON:
                player -> weapon = fitem -> id;
                break;
            case POTION:
                player -> hp += fitem -> id;
                if( player -> hp > player -> maxhp )
                    player -> hp = player -> maxhp;
                break;
            case KEY:
                player -> key++;
                break;
        }
        free( fitem );
        fitem = Next;
    }
}

Box_list *find_box( Box_list **fbox, int y, int x ) {
    Box_list *wsk = NULL;
    wsk = *fbox;
    while( ( wsk -> y != y && wsk -> x != x ) || wsk == NULL )
        wsk = wsk -> next;

    return wsk;
}


void load_maxworlds( int *max_worlds ) {
    int worlds, i = 0;
    char c;
    char number[ 5 ];
    FILE *file = fopen( "maps/worlds", "r" );
    while ( ( c = fgetc( file ) ) != EOF )
        number[ i++ ] = (char) c;
    *max_worlds = atoi( number );
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
    wrefresh( win );
}

void print_box_list( Box_list *fbox ) {
    Box_list *wsk;
    Item_list *iwsk;
    wsk = fbox;
    printw( "start\n");
    while( wsk != NULL ) {
        iwsk = wsk -> fitem;
        printw( "Box x:%d y:%d\n", wsk -> x, wsk -> y );
        while( iwsk != NULL ) {
            printw( "Item type: %d id: %d\n", iwsk -> type, iwsk -> id );
            iwsk = iwsk -> next;
        }
        wsk = wsk -> next;
    }
    printw( "end\n");
    refresh();
}
