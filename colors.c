#include <ncursesw/ncurses.h>
#include "definicje.h"

void init_colors() {
    init_color( 0, 0, 0, 0 );

    init_pair( FLOOR, COLOR_WHITE, COLOR_BLACK );
    init_pair( WALL, COLOR_BLACK, COLOR_WHITE );
    init_pair( PLAYER, COLOR_WHITE, COLOR_GREEN );
    init_pair( TELEPORT, COLOR_RED, COLOR_CYAN );
    init_pair( BOX, COLOR_WHITE, COLOR_YELLOW );
    init_pair( MONSTER, COLOR_WHITE, COLOR_MAGENTA );
    init_pair( GREEN, COLOR_WHITE, COLOR_GREEN );
    init_pair( RED, COLOR_WHITE, COLOR_RED );
}
