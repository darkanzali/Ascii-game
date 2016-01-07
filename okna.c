/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <ncursesw/ncurses.h>

#include "okna.h"

void printBorder( WINDOW *win ) {
    refresh();
    box( win, 0, 0 );
    wrefresh( win );
}
