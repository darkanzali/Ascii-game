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

#define RAT         1
#define MENEL       2
#define GOBLIN      3
#define CYCLOP      4
#define WOLF        5
#define SKELETON    6
#define TROLL       7
#define DRAGON      8
#define ZOMBIE      9

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

typedef struct {
    int type;
    int id;
    int x;
    int y;
} Mons;

#pragma pack(pop)

void add_monster( Monster *wsk, int type, int id, int x, int y ) {
    while( wsk -> next != NULL )
        wsk = wsk -> next;
    Monster *new;
    new = malloc( sizeof( Monster ) );
    wsk -> next = new;

    new -> type = type;
    new -> id   = id;
    new -> x    = x;
    new -> y    = y;
    new -> next = NULL;
}

int main( int argc, char *argv[] ) {
    if( argc != 5 ) {
        printf( "Uzycie: %s mapa.csv mapa[bez rozszerzenia] wysokosc szerokosc\n", argv[ 0 ] );
        return 1;
    }

    FILE *file;
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
    fmonster = malloc( sizeof( Monster ) );
    fmonster -> type = -1;
    fmonster -> next = NULL;

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
            case 'w':
                o.type = WALL;
                o.id   = ZERO;
                break;
            case 'G':
                o.type = PLAYER;
                o.id   = ZERO;
                xg = x + 1;
                yg = y + 1;
                break;
            case 'f':
                o.type = FLOOR;
                o.id   = ZERO;
                break;
            case 'n':
                o.type = STOP_MONSTER;
                o.id   = ZERO;
                break;
            case 'm':
                o.type = STOP_MONSTER;
                o.id   = ZERO;
                break;
            case 'Q':
                o.type = BOX;
                o.id   = ZERO;
                break;
            case 'R':
                o.type = MONSTER;
                o.id   = RAT;
                monsters++;
                if( fmonster -> type == -1 ) {
                    fmonster -> type = MONSTER;
                    fmonster -> id = RAT;
                    fmonster -> x = x;
                    fmonster -> y = y;
                } else
                    add_monster( fmonster, MONSTER, RAT, x, y );
                break;
            case 'T':
                o.type = TELEPORT;
                o.id   = ZERO;
                break;
            default:
                o.type = FLOOR;
                o.id   = ZERO;
                break;
        }
        printf( "%d", o.type );
        if( x % wid == 0 ) printf( "\n" );
        fwrite( &o, sizeof( Obj ), 1, file );
        x++;
    }
    fwrite( &yg, sizeof( int ), 1, file );
    fwrite( &xg, sizeof( int ), 1, file );
    fclose( file );

    free( f1 );
    f1 = malloc( ( strlen( argv[ 2 ] ) + 4 + 1 ) * sizeof( char ) );
    strcpy( f1, argv[ 2 ] );
    strcat( f1, "m.bin" );
    file = fopen( f1, "wb" );

    fwrite( &monsters, sizeof( int ), 1, file ); // Ilość potworów

    Monster *wsk = fmonster;
    Mons tmonster;

    while( wsk != NULL ) {
        tmonster.type = wsk -> type;
        tmonster.id   = wsk -> id;
        tmonster.x    = wsk -> x;
        tmonster.y    = wsk -> y;
        fwrite( &tmonster, sizeof( Mons ), 1, file );
        wsk = wsk -> next;
    }

    fclose( file );
    fclose( mapa );

    return 0;
}
