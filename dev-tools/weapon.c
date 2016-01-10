/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ILE 10
#define MAX_NAME 10

#pragma pack(push)
#pragma pack(1)

typedef struct { // Potwór
    int atk;
    char name[ MAX_NAME ];
    int weight;
} Weapon;

#pragma pack(pop)

int main() {
    int count = ILE;

    FILE *file;
    file = fopen( "weapons.bin", "wb" );

    fwrite( &count, sizeof( int ), 1, file );

    Weapon w[ ILE ];

    w[ 0 ].atk = 0;
    strcpy( w[ 0 ].name, "brak" );
    w[ 0 ].weight = 0;

    w[ 1 ].atk = 1;
    strcpy( w[ 1 ].name, "Kij" );
    w[ 1 ].weight = 0;

    w[ 2 ].atk = 2;
    strcpy( w[ 2 ].name, "Krótki Nóż" );
    w[ 2 ].weight = 0;

    w[ 3 ].atk = 4;
    strcpy( w[ 3 ].name, "Siekiera" );
    w[ 3 ].weight = 0;

    w[ 4 ].atk = 6;
    strcpy( w[ 4 ].name, "Topór" );
    w[ 4 ].weight = 0;

    w[ 5 ].atk = 8;
    strcpy( w[ 5 ].name, "Maczuga" );
    w[ 5 ].weight = 0;

    w[ 6 ].atk = 10;
    strcpy( w[ 6 ].name, "Maczeta" );
    w[ 6 ].weight = 0;

    w[ 7 ].atk = 12;
    strcpy( w[ 7 ].name, "Szabla" );
    w[ 7 ].weight = 0;

    w[ 8 ].atk = 13;
    strcpy( w[ 8 ].name, "Miecz" );
    w[ 8 ].weight = 0;

    w[ 9 ].atk = 60;
    strcpy( w[ 9 ].name, "Halabarda" );
    w[ 9 ].weight = 0;

    fwrite( w, sizeof( Weapon ), ILE, file );

/*
0. nic
1. Kij
2. Nóż
3. Szabla
4. Maczeta
5. Miecz
*/

    return 0;
}
