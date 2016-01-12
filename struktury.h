/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#ifndef struktury_h
#define struktury_h

#include "definicje.h"

#pragma pack(push)
#pragma pack(1)

typedef struct {
    int place;
    int color;
    int x;
    int y;
    int hp;
    int maxhp;
    int atk;
    int def;
    int weapon;
    int armor;
    int fieldch;
    int war;
    int exp;
    int lvl;
    int key;
    struct _mlist *attacking;
} Player;

typedef struct {
    char letter;
    char name[ MAX_MONSTER_NAME ];
    int hp;
    int atk;
    int weapon;
    int armor;
    int fieldch;
    int exp;
} Monster;

typedef struct {
    int atk;
    char name[ MAX_ITEM_NAME ];
    int weight;
} Weapon;

typedef struct {
    int def;
    char name[ MAX_ITEM_NAME ];
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
