#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PLACES 2
#define MAX_FILENAME 20

typedef struct el {
    char val[ MAX_FILENAME ];
    struct el *next;
} Element;

void add( Element *first, char tab[ MAX_FILENAME ] ) {
    Element *wsk = first, *new;
    while( wsk -> next != NULL )
        wsk = wsk -> next;

    wsk -> next = new;
    strcpy( new -> val, tab );
    new -> next = NULL;
}

int main() {
    int placesCount = PLACES;
    int currentPlace = 1;

    Element *first;

    first -> val = "start.bin\0";
    first -> next = NULL;

    add( first, "zadanie1.bin\0" );
    add( first, "zadanie2.bin\0" );

    FILE *file;
    file = fopen( "settings.bin", "wb" );

    Element *curr = first;

    while( curr != NULL ) {
        fwrite( curr -> val, sizeof())
    }

    return 0;
}
