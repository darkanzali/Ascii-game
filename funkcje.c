/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#include <sys/time.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
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

char *l_to_char( int hp, int maxhp ) {
	char *text = malloc( 11 * sizeof( char ) );
	int len, tmp, i;

	tmp = hp;
	i = 0;
	do {
		i++;
		tmp /= 10;
	} while( tmp != 0 );

	text[ i ] = '/';
	text[ i + 1 ] = 0;
	i--;
	tmp = hp;
	while( i >= 0 ) {
		text[ i ] = ( tmp % 10 ) + '0';
		tmp /= 10;
		i--;
	}

	len = strlen( text );
	i = len;
	len--;
	tmp = maxhp;

	do {
		i++;
		tmp /=10;
	} while( tmp != 0 );

	tmp = maxhp;
	text[ i ] = 0;
	i--;

	while( i > len ) {
		text[ i ] = ( tmp % 10 ) + '0';
		tmp /= 10;
		i--;
	}

	return text;
}
