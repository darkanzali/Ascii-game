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

char *world_to_char( char *dir, int number, char *ext ) {
	char id[ 3 ] = { 0, 0, 0 };

    if( number > 9 ) {
        id[ 1 ] = ( number % 10 ) + '0';
        number /= 10;
    }
    id[ 0 ] = number + '0';

    char *map;
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


int more_random( long max ) {
    unsigned long
    num_bins = ( unsigned long ) max + 1,
    num_rand = ( unsigned long ) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

    long x;
    do {
        x = random();
    } while( num_rand - defect <= ( unsigned long ) x );

    return x / bin_size;
}

int time_diff( struct timeval start, struct timeval end ) {
    long difference, seconds, useconds;
    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;
    difference = ( ( ( seconds ) * 1000 + useconds / 1000.0 ) + 0.5 );
    return difference;
}
