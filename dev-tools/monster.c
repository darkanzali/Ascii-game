/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ILE         10
#define MAX_MONSTER_NAME 10

#pragma pack(push)
#pragma pack(1)

typedef struct { // Potwór
    char letter; // Literka identyfikująca rodzaj potwora
    char name[ MAX_MONSTER_NAME ]; // Nazwa potwora
    int hp; // Życie potwora
    int atk; // Siła potwora
    int weapon; // Id broni w ręku potwora
    int armor; // Id zbroi którą nosi potwór
    int fieldch; // Znak pola na którym stoi potwór
    int exp;
} Monster;

#pragma pack(pop)

int main() {
    int count = ILE;

    FILE *file;
    file = fopen( "monsters.bin", "wb" );

    fwrite( &count, sizeof( int ), 1, file );

    Monster m[ ILE ];

    m[ 0 ].letter   = 'S';
    strcpy( m[ 0 ].name, "Szczur" );
    m[ 0 ].hp       = 10;
    m[ 0 ].atk      = 2;
    m[ 0 ].weapon   = 0;
    m[ 0 ].armor    = 0;
    m[ 0 ].fieldch  = '.';
    m[ 0 ].exp      = 1;

    m[ 1 ].letter   = 'M';
    strcpy( m[ 1 ].name, "Menel" );
    m[ 1 ].hp       = 25;
    m[ 1 ].atk      = 5;
    m[ 1 ].weapon   = 0;
    m[ 1 ].armor    = 0;
    m[ 1 ].fieldch  = '.';
    m[ 1 ].exp      = 1;

    m[ 2 ].letter   = 'G';
    strcpy( m[ 2 ].name, "Goblin" );
    m[ 2 ].hp       = 40;
    m[ 2 ].atk      = 7;
    m[ 2 ].weapon   = 0;
    m[ 2 ].armor    = 0;
    m[ 2 ].fieldch  = '.';
    m[ 2 ].exp      = 1;

    m[ 3 ].letter   = 'W';
    strcpy( m[ 3 ].name, "Wilk" );
    m[ 3 ].hp       = 55;
    m[ 3 ].atk      = 10;
    m[ 3 ].weapon   = 0;
    m[ 3 ].armor    = 0;
    m[ 3 ].fieldch  = '.';
    m[ 3 ].exp      = 1;

    m[ 4 ].letter   = 'C';
    strcpy( m[ 4 ].name, "Cyklop" );
    m[ 4 ].hp       = 70;
    m[ 4 ].atk      = 12;
    m[ 4 ].weapon   = 0;
    m[ 4 ].armor    = 0;
    m[ 4 ].fieldch  = '.';
    m[ 4 ].exp      = 1;

    m[ 5 ].letter   = 'X';
    strcpy( m[ 5 ].name, "Szkielet" );
    m[ 5 ].hp       = 85;
    m[ 5 ].atk      = 15;
    m[ 5 ].weapon   = 0;
    m[ 5 ].armor    = 0;
    m[ 5 ].fieldch  = '.';
    m[ 5 ].exp      = 1;

    m[ 6 ].letter   = 'T';
    strcpy( m[ 6 ].name, "Troll" );
    m[ 6 ].hp       = 100;
    m[ 6 ].atk      = 17;
    m[ 6 ].weapon   = 0;
    m[ 6 ].armor    = 0;
    m[ 6 ].fieldch  = '.';
    m[ 6 ].exp      = 1;

    m[ 7 ].letter   = 'Z';
    strcpy( m[ 7 ].name, "Zombie" );
    m[ 7 ].hp       = 115;
    m[ 7 ].atk      = 19;
    m[ 7 ].weapon   = 0;
    m[ 7 ].armor    = 0;
    m[ 7 ].fieldch  = '.';
    m[ 7 ].exp      = 1;

    m[ 8 ].letter   = 'D';
    strcpy( m[ 8 ].name, "Smok" );
    m[ 8 ].hp       = 10;
    m[ 8 ].atk      = 100;
    m[ 8 ].weapon   = 0;
    m[ 8 ].armor    = 0;
    m[ 8 ].fieldch  = '.';
    m[ 8 ].exp      = 1;

    m[ 9 ].letter   = 'A';
    strcpy( m[ 9 ].name, "END" );
    m[ 9 ].hp       = -1;
    m[ 9 ].atk      = -1;
    m[ 9 ].weapon   = -1;
    m[ 9 ].armor    = -1;
    m[ 9 ].fieldch  = 'A';
    m[ 9 ].exp      = -1;

    fwrite( m, sizeof( Monster ), ILE, file );


    /*  1. szczur S
        2. menel M
        3. goblin G
        4. wilk W
        5. cyklop C
        6. szkielet X
        7. troll T
        8. zombie Z
        9. smok D
    */

    return 0;
}
