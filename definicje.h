#ifndef definicje_h
#define definicje_h

#define true 1
#define false 0

#define MONSTERSFILE    "files/monsters.bin"
#define ARMORSFILE      "files/armors.bin"
#define WEAPONSFILE     "files/weapons.bin"

#define FLOOR           1
#define WALL            2
#define STOP_MONSTER    3
#define PLAYER          4
#define MONSTER         5
#define BOX             6
#define TELEPORT        7
#define DOOR            8

#define GREEN           10
#define RED             11

#define FLOOR_CH        '.'
#define WALL_CH         '#'
#define STOP_MONSTER_CH '\"'
#define PLAYER_CH       '@'
#define BOX_CH          '&'
#define TELEPORT_CH     '*'
#define DOOR_CH         'L'

#define ZERO                0
#define MAX_X               100
#define MAX_Y               100
#define PLACES              10
#define MAX_PLACE_NAME      20
#define MAX_ITEM_NAME       10
#define MAX_MONSTER_NAME    10
#define MAX_MAP_FILENAME    15

#define ARMOR 1
#define WEAPON 2
#define POTION 3
#define KEY 4

#define won( x, y )  wattron( x, COLOR_PAIR( y ) )
#define woff( x, y ) wattroff( x, COLOR_PAIR( y ) )

/****************
Kolory:

COLOR_BLACK     0
COLOR_RED       1
COLOR_GREEN     2
COLOR_YELLOW    3
COLOR_BLUE      4
COLOR_MAGENTA   5
COLOR_CYAN      6
COLOR_WHITE     7
*****************/

#endif
