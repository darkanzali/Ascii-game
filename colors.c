#include <ncursesw/ncurses.h>
#include "definicje.h"

void init_colors() {
    init_pair( FLOOR, COLOR_WHITE, COLOR_BLACK );
    init_pair( WALL, COLOR_BLACK, COLOR_WHITE );
    init_pair( PLAYER, COLOR_WHITE, COLOR_GREEN );
    init_pair( TELEPORT, COLOR_RED, COLOR_CYAN );
    init_pair( BOX, COLOR_WHITE, COLOR_MAGENTA );
}
