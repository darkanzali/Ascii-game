/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ZERO        0

#define FLOOR           1
#define WALL            2
#define STOP_MONSTER    3
#define PLAYER          4
#define MONSTER         5
#define BOX             6
#define TELEPORT        7
#define DOOR            8

#define MAX_MONSTER_NAME 10

#define MONSTERSFILE    "monsters.bin"

#pragma pack(push)
#pragma pack(1)

typedef struct {
    int type;
    int id;
} Obj;

typedef struct mon {
    int type;
    int id;
    int x;
    int y;
    struct mon *next;
} Monster;

typedef struct { // Potwór
    char letter; // Literka identyfikująca rodzaj potwora
    char name[ MAX_MONSTER_NAME ]; // Nazwa potwora
    int hp; // Życie potwora
    int atk; // Siła potwora
    int weapon; // Id broni w ręku potwora
    int armor; // Id zbroi którą nosi potwór
    int fieldch; // Znak na polu na którym stoi potwór
    int exp;
} Mon;

typedef struct {
    int type;
    int id;
    int x;
    int y;
} Mons;

#pragma pack(pop)

void add_monster( Monster **fmonster, int type, int id, int x, int y ) {
    Monster *wsk, *new;

    if( *fmonster == NULL ) {
        ( *fmonster ) = malloc( sizeof( Monster ) );
        ( *fmonster ) -> next = NULL;
        new = *fmonster;
    } else {
        wsk = *fmonster;
        while( wsk -> next != NULL )
            wsk = wsk -> next;

        new = malloc( sizeof( Monster ) );
        new -> next = NULL;
        wsk -> next = new;
    }

    new -> type = type;
    new -> id   = id;
    new -> x    = x;
    new -> y    = y;
}

void load_monsters( Mon **monsters ) {
    FILE *file;
    file = fopen( MONSTERSFILE, "rb" );
    int count;
    fread( &count, sizeof( int ), 1, file );
    *monsters = malloc( count * sizeof( Mon ) );
    fread( *monsters, sizeof( Mon ), count, file );
    fclose( file );
}

int main( int argc, char *argv[] ) {
    if( argc != 5 ) {
        printf( "Uzycie: %s mapa.csv mapa[bez rozszerzenia] wysokosc szerokosc\n", argv[ 0 ] );
        return 1;
    }

    FILE *file;
    Mon *amonsters;

    file = fopen( MONSTERSFILE, "rb" );
    load_monsters( &amonsters );
    int im;
    fclose( file );

    char *f1;
    f1 = malloc( ( strlen( argv[ 2 ] ) + 3 + 1 ) * sizeof( char ) );
    strcpy( f1, argv[ 2 ] );
    strcat( f1, ".bin" );

    file = fopen( f1, "wb" );

    int hei = atoi( argv[ 3 ] ), wid = atoi( argv[ 4 ] );
    int xg, yg;
    int x, y;
    int monsters;
    Monster *fmonster;
    fmonster = NULL;

    fwrite( &hei, sizeof( int ), 1, file );
    fwrite( &wid, sizeof( int ), 1, file );

    y=1;
    x=1;

    FILE *mapa;
    mapa = fopen( argv[ 1 ], "r" );

    char c;
    Obj o;

    while( ( c = fgetc( mapa ) ) != EOF ) {
        if( c == '\n' ) {
            y++;
            continue;
        }
        getc( mapa );
        switch( c ) {
            case '#':
                o.type = WALL;
                o.id   = ZERO;
                break;
            case '@':
                o.type = PLAYER;
                o.id   = ZERO;
                xg = x + 1;
                yg = y + 1;
                break;
            case 'L':
                o.type = DOOR;
                o.id   = ZERO;
                break;
            case 'f':
                o.type = FLOOR;
                o.id   = ZERO;
                break;
            case 'n':
                o.type = STOP_MONSTER;
                o.id   = ZERO;
                break;
            case '&':
                o.type = BOX;
                o.id   = ZERO;
                printf( "Box - x: %d, y: %d\n", x % wid, y );
                break;
            case '*':
                o.type = TELEPORT;
                o.id   = ZERO;
                break;
            default:
                o.type = FLOOR;
                o.id   = ZERO;
                for( im = 0; amonsters[ im ].hp != -1; im++ ) {
                    if( amonsters[ im ].letter == c ) {
                        o.type = MONSTER;
                        o.id   = im + 1;
                        //add_monster( &fmonster, o.type, o.id, x, y );
                        break;
                    }
                }
                break;
        }
        //printf( "%d", o.type );
        //if( x % wid == 0 ) printf( "\n" );
        fwrite( &o, sizeof( Obj ), 1, file );
        x++;
    }
    fwrite( &yg, sizeof( int ), 1, file );
    fwrite( &xg, sizeof( int ), 1, file );
    fclose( file );

    free( f1 );
    // f1 = malloc( ( strlen( argv[ 2 ] ) + 4 + 1 ) * sizeof( char ) );
    // strcpy( f1, argv[ 2 ] );
    // strcat( f1, "m.bin" );
    // file = fopen( f1, "wb" );
    //
    // fwrite( &monsters, sizeof( int ), 1, file ); // Ilość potworów
    //
    // Monster *wsk = fmonster;
    // Mons tmonster;
    //
    // while( wsk != NULL ) {
    //     tmonster.type = wsk -> type;
    //     tmonster.id   = wsk -> id;
    //     tmonster.x    = wsk -> x;
    //     tmonster.y    = wsk -> y;
    //     fwrite( &tmonster, sizeof( Mons ), 1, file );
    //     wsk = wsk -> next;
    // }
    //
    // fclose( file );
    fclose( mapa );

    return 0;
}
