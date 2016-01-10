/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

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
    int def;
    int weapon; // Id broni w ręku gracza
    int armor; // Id zbroi którą nosi gracz
    int fieldch; // Znak na polu na którym stoi gracz
    int war; // Czy gracz walczy z potworem
    int exp;
    int lvl;
    int key;
    struct _mlist *attacking; // Wskaźnik na potwora z którym walczymy
} Player;

typedef struct { // Potwór
    char letter; // Literka identyfikująca rodzaj potwora
    char name[ MAX_MONSTER_NAME ]; // Nazwa potwora
    int hp; // Życie potwora
    int atk; // Siła potwora
    int weapon; // Id broni w ręku potwora
    int armor; // Id zbroi którą nosi potwór
    int fieldch; // Znak na polu na którym stoi potwór
    int exp;
} Monster;

typedef struct { // Broń
    int atk; // Moc ataku broni
    char name[ MAX_ITEM_NAME ]; // Nazwa broni
    int weight;
} Weapon;

typedef struct { // Zbroja
    int def; // Moc absorbowania ataku przez zbroję
    char name[ MAX_ITEM_NAME ]; // Nazwa zbroi
    int weight;
} Armor;

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
    int maxhp;
    int atk;
    int weapon;
    int armor;
    int fieldch;
    int exp;
    int war;
    int x;
    int y;
    struct _mlist *next;
    struct _mlist *prev;
} Monster_list;

typedef struct {
    WINDOW *mwin;
    WINDOW *twin;
    WINDOW *rwin;
    WINDOW *ewin;
    WINDOW *dwin;
} Windows;

typedef struct _blist {
    int y;
    int x;
    struct _ilist *fitem;
    struct _blist *next;
} Box_list;

typedef struct _ilist {
    int type;
    int id;
    struct _ilist *next;
} Item_list;

typedef struct {
    int type;
    int id;
} Item;

#pragma pack(pop)

#endif
