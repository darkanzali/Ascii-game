#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ILE         5
#define NO_ATK      1
#define ATK_IF_ATK  2
#define DEFAULT_ATK 2
#define MAX_MONSTER_NAME 10

#pragma pack(push)
#pragma pack(1)

typedef struct { // Potwór
    int color; // Kolor kratki reprezentującej potwora
    char letter; // Literka identyfikująca rodzaj potwora
    char name[ MAX_MONSTER_NAME ]; // Nazwa potwora
    int hp; // Życie potwora
    int weapon; // Id broni w ręku potwora
    int armor; // Id zbroi którą nosi potwór
} Monster;

#pragma pack(pop)

int main() {
    int count = ILE;

    FILE *file;
    file = fopen( "monsters.bin", "wb" );

    fwrite( &count, sizeof( int ), 1, file );

    Monster m[ ILE ];

    m[ 0 ].color    = ATK_IF_ATK;
    m[ 0 ].letter   = 'R';
    strcpy( m[ 0 ].name, "Szczur" );
    m[ 0 ].hp       = 20;
    m[ 0 ].weapon   = 0;
    m[ 0 ].armor    = 0;

    m[ 1 ].color    = NO_ATK;
    m[ 1 ].letter   = 'P';
    strcpy( m[ 1 ].name, "Swinia" );
    m[ 1 ].hp       = 10;
    m[ 1 ].weapon   = 0;
    m[ 1 ].armor    = 0;

    m[ 2 ].color    = NO_ATK;
    m[ 2 ].letter   = 'K';
    strcpy( m[ 2 ].name, "Krowa" );
    m[ 2 ].hp       = 10;
    m[ 2 ].weapon   = 0;
    m[ 2 ].armor    = 0;

    m[ 3 ].color    = ATK_IF_ATK;
    m[ 3 ].letter   = 'C';
    strcpy( m[ 3 ].name, "Cyklop" );
    m[ 3 ].hp       = 10;
    m[ 3 ].weapon   = 0;
    m[ 3 ].armor    = 0;

    m[ 4 ].color    = DEFAULT_ATK;
    m[ 4 ].letter   = 'W';
    strcpy( m[ 4 ].name, "Wilk" );
    m[ 4 ].hp       = 10;
    m[ 4 ].weapon   = 0;
    m[ 4 ].armor    = 0;

    fwrite( m, sizeof( Monster ), ILE, file );


/*  1. szczur   rat         CYAN
    2. świnia   pig         GREEN
    3. krowa    cow         GREEN
    4. cyklop   cyclop      CYAN
    5. wilk     wolf        RED
    6. szkielet skeleton    RED
    7. troll    troll       RED
    8. smok     dragon      RED
    9. zombie   zombie      RED
*/

    return 0;
}
