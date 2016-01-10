/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#ifndef funkcje_h
#define funkcje_h

int kbhit(void);
char *world_to_char( char *dir, int number, char *ext );
char *l_to_char( int hp, int maxhp );
char *box_name( char *dir, int world, int y, int x, char *ext );
int more_random( long max );
int time_diff( struct timeval start, struct timeval end );

#endif
