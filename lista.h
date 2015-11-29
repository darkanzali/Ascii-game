#ifndef lista_h
#define lista_h

#pragma pack(push)
#pragma pack(1)

struct element {
    int type; // Typ obiektu
    int id; // Id obiektu
    int x; // Współrzędna x'owa obiektu
    int y; // Współrzędna y'owa obiektu

    struct element *next;
    struct element *prev;
};

struct element *first = NULL;
struct element *last = NULL;

#pragma pack(pop)

#endif
