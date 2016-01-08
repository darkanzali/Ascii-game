/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ILE 6
#define MAX_NAME 10

#pragma pack(push)
#pragma pack(1)

typedef struct { // Skrzynka
    int type;
    int id;
} Item;

#pragma pack(pop)

int main( int argc, char *argv[] ) {
    if( argc != 2 ) {
        printf( "Uzycie: %s plik_do_zapisu\n", argv[ 0 ] );
        return 1;
    }
    int count = atoi( argv[ 1 ] );
    int many, i, j, obj, x, y;
    Item it;

    FILE *file;
    file = fopen( argv[ 1 ], "wb" );

    printf( "Ile skrzynek dodajemy? " );
    scanf( "%d", &many );
    //many++;

    fwrite( &many, sizeof( int ), 1, file );

    printf( "Wpisuj dla %d kolejnych skrzynek:\n", many );
    printf( "y x ilosc_obiektow_w_niej\n" );
    printf( "potem type i id\n" );
    for( i = 0; i < many; i++ ){
        scanf( "%d %d %d", &y, &x, &obj );
        printf( "dodajemy %d rzeczy do skrzynki o wspolrzednych y: %d, x: %d\n", obj, y, x );
        fwrite( &y, sizeof( int ), 1, file );
        fwrite( &x, sizeof( int ), 1, file );
        fwrite( &obj, sizeof( int ), 1, file );
        for( j = 0; j < obj; j++ ) {
            printf( "Obj %d: ", j + 1 );
            scanf( "%d %d", &it.type, &it.id );
            printf( "%d %d\n", it.type, it.id );
            fwrite( &it, sizeof( Item ), 1, file );
        }
    }

    //it.type = -1;
    //it.id   = -1;
    //fwrite( &it, sizeof( Item ), 1, file );

    fclose( file );

    printf( "\n" );

    return 0;
}
