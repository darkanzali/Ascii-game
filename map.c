#include <stdio.h>
#include <stdlib.h>

#pragma pack(push)
#pragma pack(1)
typedef struct {
    int type;
    int id;
} Mapa;
#pragma pack(pop)

int main() {
    FILE *file;
    file = fopen( "map.dat", "wb" );
    Mapa mapa;
    mapa.type = 1;
    mapa.id = 0;
    fwrite( &mapa, sizeof(Mapa), 1, file );
    mapa.type = 2;
    mapa.id = 0;
    fwrite( &mapa, sizeof(Mapa), 1, file );
    mapa.type = 1;
    mapa.id = 0;
    fwrite( &mapa, sizeof(Mapa), 1, file );
    int a=10;
    printf("done\n");
    fclose;

    return 0;
}
