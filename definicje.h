#ifndef definicje_h
#define definicje_h

#define true 1
#define false 0

#define MAPFILE         "files/1.bin"
#define MONSTERSFILE    "files/monsters.bin"

#define FLOOR           1
#define WALL            2
#define STOP_MONSTER    3
#define PLAYER          4
#define MONSTER         5
#define BOX             6
#define TELEPORT        7

#define FLOOR_CH        '.'
#define WALL_CH         '#'
#define STOP_MONSTER_CH '\"'
#define PLAYER_CH       '@'
#define BOX_CH          '&'
#define TELEPORT_CH     'T'

#define ZERO                0
#define MAX_X               100 // Maksymalna szerokość miejsc
#define MAX_Y               100 // Maksymalna wysokość miejsc
#define PLACES              10 // Ilość miejsc na mapie
#define MAX_PLACE_NAME      20 // Maksymalna długość nazwy miejsca
#define MAX_ITEM_NAME       10 // Maksymalna długość nazwy przedmiotów w grze
#define MAX_MONSTER_NAME    10 // Maksymalna długość nazwy potwora

#endif
