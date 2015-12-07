#ifndef struktury_h
#define struktury_h

#define MAX_X 100 // Maksymalna szerokość miejsc
#define MAX_Y 100 // Maksymalna wysokość miejsc
#define PLACES 10 // Ilość miejsc na mapie
#define MAX_PLACE_NAME 20 // Maksymalna długość nazwy miejsca
#define MAX_ITEM_NAME 10 // Maksymalna długość nazwy przedmiotów w grze
#define MAX_MONSTER_NAME 10 // Maksymalna długość nazwy potwora

#pragma pack(push)
#pragma pack(1)

typedef struct { // Gracz
    int place; // Nazwa miejsca w którym znajduje się gracz
    int color; // Kolor kratki z graczem
    int x; // Pozycja x'owa gracza
    int y; // Pozycja y'owa gracza
    int hp; // Życie gracza
    int weapon; // Id broni w ręku gracza
    int armor; // Id zbroi którą nosi gracz
} Player;

typedef struct mnstr { // Potwór
    int color; // Kolor kratki reprezentującej potwora
    char letter; // Literka identyfikująca rodzaj potwora
    char name[ MAX_MONSTER_NAME ]; // Nazwa potwora
    int hp; // Życie potwora
    int x;
    int y;
    int weapon; // Id broni w ręku potwora
    int armor; // Id zbroi którą nosi potwór
    struct mnstr *next;
} Monster;

typedef struct { // Broń
    int atk; // Moc ataku broni
    char name[ MAX_ITEM_NAME ]; // Nazwa broni
} Weapons;

typedef struct { // Zbroja
    int def; // Moc absorbowania ataku przez zbroję
    char name[ MAX_ITEM_NAME ]; // Nazwa zbroi
} Armors;

typedef struct mp {
    int type;
    int id;
    struct mp *next;
} Map; // Mapa

#pragma pack(pop)

#endif
