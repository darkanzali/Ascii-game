/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ILE 7
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

    w[ 2 ].atk = 3;
    strcpy( w[ 2 ].name, "Nóż" );
    w[ 2 ].weight = 0;

    w[ 3 ].atk = 7;
    strcpy( w[ 3 ].name, "Szabla" );
    w[ 3 ].weight = 0;

    w[ 4 ].atk = 10;
    strcpy( w[ 4 ].name, "Maczeta" );
    w[ 4 ].weight = 0;

    w[ 5 ].atk = 20;
    strcpy( w[ 5 ].name, "Miecz" );
    w[ 5 ].weight = 0;

    w[ 6 ].atk = -1;
    strcpy( w[ 6 ].name, "END" );
    w[ 6 ].weight = -1;

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
