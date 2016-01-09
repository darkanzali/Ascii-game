/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#ifndef game_h
#define game_h

int playGame( int world, Windows w );
void print_map( int map, Player *player, Monster *monsters, Monster_list **fmonster_on_map, Box_list **fbox, WINDOW *win, WINDOW *twin ); // Funkcja do wypisania całej mapy
void add_to_item_list( Item_list **fitem, Item item );
void add_box_to_list( Box_list **fbox, int y, int x, Item_list *fitem );
void prplayer_xy( WINDOW *win, int y, int x ); // Funkcja wypisująca gracza na mapie
void prfch_xy( WINDOW *win, char ch, int y, int x ); // Funkcja wypisująca znak na mapie
void prmon_xy( WINDOW *win, Monster *monsters, int id, int y, int x ); // Funkcja wypisująca potwora na mapie
void prlife( WINDOW *win, Player player, Monster_list *monster );
void load_monsters( Monster **monsters ); // Funkcja wczytująca potwory z pliku do tablicy
void load_weapons( Weapon **weapons );
void load_armors( Armor **armors );
void add_monster( Monster *monsters, Monster_list **fmonster, int id, int uniId, int y, int x ); // Funkcja dodająca potwora na mapę
void add_monster_saved( Monster_list mToAdd, Monster_list **fmonster, int uniId ); // Funkcja dodająca potwora wczytanego z zapisu
void delete_dead_monster( WINDOW *win, Monster_list **pointer, Monster_list **fmonster ); // Funkcja usuwająca potwora z listy
void clear_list( Monster_list **monster ); // Funkcja czyszcząca listę
void clear_list_box( Box_list **fbox );
Monster_list *checkIfMonsterNearPlayer( Player *player, Monster_list *fmonster ); // Funkcja sprawdzająca czy potwór jest obok gracza
void save_game( WINDOW *win, int world, Player player, Monster_list *fmonster, Monster *monsters );
int load_player( Player *player );
int load_saved_game( Player *player, Monster *monsters, Monster_list **fmonster, WINDOW *win );
void check_player_exp( Player *player );
void print_player_info( WINDOW *win, Player player, Armor *armors, Weapon *weapons );
void open_box( Player *player, int y, int x, Box_list **fbox, WINDOW *win );
Box_list *find_box( Box_list **fbox, int y, int x );
void print_list( Monster_list *fmonster, WINDOW *win ); //DEBUG
void print_box_list( Box_list *fbox ); // DEBUG

#endif
