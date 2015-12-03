#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FILECSV     "mapa1.csv"
#define WIDTH       20
#define HEIGHT      10
#define XG          20
#define YG          10

#define ZERO        0

#define FLOOR           1
#define WALL            2
#define STOP_MONSTER    3
#define PLAYER          4
#define MONSTER         5
#define BOX             1

#define RAT         1
#define PIG         2
#define COW         3
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
    int x;
    int y;
} Obj;

#pragma pack(pop)

int main( int argc, char *argv[] ) {
    if( argc != 3 ) {
        printf( "Uzycie: %s mapa.csv mapa.bin\n", argv[ 0 ] );
        return 1;
    }

    FILE *file;
    file = fopen( argv[ 2 ], "wb" );

    int hei = HEIGHT, wid = WIDTH;
    int xg = XG, yg = YG;
    int x, y;

    fwrite( &hei, sizeof( int ), 1, file );
    fwrite( &wid, sizeof( int ), 1, file );
    fwrite( &yg, sizeof( int ), 1, file );
    fwrite( &xg, sizeof( int ), 1, file );

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
                o.x    = x;
                o.y    = y;
                break;
            case 'G':
                o.type = PLAYER;
                o.id   = ZERO;
                o.x    = x;
                o.y    = y;
                break;
            case 'f':
                o.type = FLOOR;
                o.id   = ZERO;
                o.x    = x;
                o.y    = y;
                break;
            case 'n':
                o.type = STOP_MONSTER;
                o.id   = ZERO;
                o.x    = x;
                o.y    = y;
                break;
            case 'm':
                o.type = STOP_MONSTER;
                o.id   = ZERO;
                o.x    = x;
                o.y    = y;
                break;
            case 'Q':
                o.type = BOX;
                o.id   = ZERO;
                o.x    = x;
                o.y    = y;
                break;
            case 'R':
                o.type = MONSTER;
                o.id   = RAT;
                o.x    = x;
                o.y    = y;
                break;
            default:
                o.type = FLOOR;
                o.id   = ZERO;
                o.x    = x;
                o.y    = y;
                break;
        }
        fwrite( &o, sizeof( Obj ), 1, file );
        x++;
    }

    return 0;
}
