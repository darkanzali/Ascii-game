#ifndef game_h
#define game_h

int playGame( int world, Windows w );
void print_map( int map, Player *player, Monster *monsters, Monster_list **fmonster_on_map, WINDOW *win, WINDOW *twin ); // Funkcja do wypisania całej mapy
void prplayer_xy( WINDOW *win, int y, int x ); // Funkcja wypisująca gracza na mapie
void prfch_xy( WINDOW *win, char ch, int y, int x ); // Funkcja wypisująca znak na mapie
void prmon_xy( WINDOW *win, Monster *monsters, int id, int y, int x ); // Funkcja wypisująca potwora na mapie
void prlife( WINDOW *win, Player player, Monster_list *monster );
void load_monsters( Monster **monsters, WINDOW *win ); // Funkcja wczytująca potwory z pliku do tablicy
void add_monster( Monster *monsters, Monster_list **fmonster, int id, int uniId, int y, int x ); // Funkcja dodająca potwora na mapę
void add_monster_saved( Monster_list mToAdd, Monster_list **fmonster, int uniId ); // Funkcja dodająca potwora wczytanego z zapisu
void delete_dead_monster( WINDOW *win, Monster_list **pointer, Monster_list **fmonster ); // Funkcja usuwająca potwora z listy
void clear_list( Monster_list **monster ); // Funkcja czyszcząca listę
Monster_list *checkIfMonsterNearPlayer( Player *player, Monster_list *fmonster ); // Funkcja sprawdzająca czy potwór jest obok gracza
int time_diff( struct timeval start, struct timeval end );
int more_random( long max );
void save_game( WINDOW *win, int world, Player player, Monster_list *fmonster, Monster *monsters );
void load_saved_game( Player *player, Monster *monsters, Monster_list **fmonster, WINDOW *win );
void print_list( Monster_list *fmonster, WINDOW *win ); //DEBUG

#endif
