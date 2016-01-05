#include <sys/time.h> /* struct timeval, select() */
#include <termios.h> /* tcgetattr(), tcsetattr() */
#include <stdlib.h> /* atexit(), exit() */
#include <unistd.h> /* read() */
#include <stdio.h> /* printf() */
#include <ncursesw/ncurses.h>
#include <string.h>

#include "definicje.h"

int kbhit(void)
{
	struct timeval timeout;
	fd_set read_handles;
	int status;

	raw();
	FD_ZERO(&read_handles);
	FD_SET(0, &read_handles);
	timeout.tv_sec = timeout.tv_usec = 0;
	status = select(0 + 1, &read_handles, NULL, NULL, &timeout);
	if(status < 0)
	{
		printf("select() failed in kbhit()\n");
		exit(1);
	}
	noraw();
	cbreak();

	return status;
}

char *world_to_char( char dir[], int number, char ext[] ) {
	char id[ 3 ] = { 0, 0, 0 };

    if( number > 9 ) {
        id[ 1 ] = ( number % 10 ) + '0';
        number /= 10;
    }
    id[ 0 ] = number + '0';

    char *map = NULL;
	map = malloc( MAX_MAP_FILENAME * sizeof( char ) );

    strcpy( map, dir );
    strcat( map, id );
    strcat( map, ext );

	return map;
}
