#ifndef struktury_h
#define struktury_h

#include "definicje.h"

#pragma pack(push)
#pragma pack(1)

typedef struct { // Gracz
    int place; // Nazwa miejsca w którym znajduje się gracz
    int color; // Kolor kratki z graczem
    int x; // Pozycja x'owa gracza
    int y; // Pozycja y'owa gracza
    int hp; // Życie gracza
    int maxhp; // Maksymalne życie gracza
    int atk; // Siła gracza
    int weapon; // Id broni w ręku gracza
    int armor; // Id zbroi którą nosi gracz
    int fieldch; // Znak na polu na którym stoi gracz
    int war;
} Player;

typedef struct { // Potwór
    char letter; // Literka identyfikująca rodzaj potwora
    char name[ MAX_MONSTER_NAME ]; // Nazwa potwora
    int hp; // Życie potwora
    int atk; // Siła potwora
    int weapon; // Id broni w ręku potwora
    int armor; // Id zbroi którą nosi potwór
    int fieldch; // Znak na polu na którym stoi potwór
} Monster;

typedef struct { // Broń
    int atk; // Moc ataku broni
    char name[ MAX_ITEM_NAME ]; // Nazwa broni
} Weapons;

typedef struct { // Zbroja
    int def; // Moc absorbowania ataku przez zbroję
    char name[ MAX_ITEM_NAME ]; // Nazwa zbroi
} Armors;

typedef struct {
    int type;
    int id;
} Map_field;

typedef struct _mlist {
    int id;
    int uniId;
    char letter;
    char name[ MAX_MONSTER_NAME ];
    int hp;
    int atk;
    int weapon;
    int armor;
    int fieldch;
    int war;
    int x;
    int y;
    struct _mlist *next;
    struct _mlist *prev;
} Monster_list;

#pragma pack(pop)

#endif
