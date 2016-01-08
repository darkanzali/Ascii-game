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

typedef struct { // Potwór
    int def;
    char name[ MAX_NAME ];
    int weight;
} Armor;

#pragma pack(pop)

int main() {
    int count = ILE;

    FILE *file;
    file = fopen( "armors.bin", "wb" );

    fwrite( &count, sizeof( int ), 1, file );

    Armor a[ ILE ];

    a[ 0 ].def = 0;
    strcpy( a[ 0 ].name, "brak" );
    a[ 0 ].weight = 0;

    a[ 1 ].def = 1;
    strcpy( a[ 1 ].name, "Płaszcz" );
    a[ 1 ].weight = 0;

    a[ 2 ].def = 3;
    strcpy( a[ 2 ].name, "Skóra" );
    a[ 2 ].weight = 0;

    a[ 3 ].def = 7;
    strcpy( a[ 3 ].name, "Kolczuga" );
    a[ 3 ].weight = 0;

    a[ 4 ].def = 10;
    strcpy( a[ 4 ].name, "Płytowy" );
    a[ 4 ].weight = 0;

    a[ 5 ].def = 20;
    strcpy( a[ 5 ].name, "Platynowy" );
    a[ 5 ].weight = 0;

    fwrite( a, sizeof( Armor ), ILE, file );

/*
0. nic
1. Płaszcz
2. Skóra
3. Kolczuga
4. Płytowy
5. Platynowy
*/

    return 0;
}
