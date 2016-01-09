#ifndef definicje_h
#define definicje_h

/* Tworzenie zmiennych bool */

#define true 1
#define false 0

/* Pliki z danymi do gry */
#define MONSTERSFILE    "files/monsters.bin"
#define ARMORSFILE      "files/armors.bin"
#define WEAPONSFILE     "files/weapons.bin"

/* Obiekty na mapie */

#define FLOOR           1
#define WALL            2
#define STOP_MONSTER    3
#define PLAYER          4
#define MONSTER         5
#define BOX             6
#define TELEPORT        7
#define GREEN           10
#define RED             11

#define FLOOR_CH        '.'
#define WALL_CH         '#'
#define STOP_MONSTER_CH '\"'
#define PLAYER_CH       '@'
#define BOX_CH          '&'
#define TELEPORT_CH     'T'

/* Definicje potrzebne do działania gry */

#define ZERO                0
#define MAX_X               100 // Maksymalna szerokość miejsc
#define MAX_Y               100 // Maksymalna wysokość miejsc
#define PLACES              10 // Ilość miejsc na mapie
#define MAX_PLACE_NAME      20 // Maksymalna długość nazwy miejsca
#define MAX_ITEM_NAME       10 // Maksymalna długość nazwy przedmiotów w grze
#define MAX_MONSTER_NAME    10 // Maksymalna długość nazwy potwora
#define MAX_MAP_FILENAME    15 // Maksymalna długość nazwy pliku z mapą

/* Itemy */

#define ARMOR 1
#define WEAPON 2
#define POTION 3
#define KEY 4

/* Funkcje do włączania kolorów */

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
