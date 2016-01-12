/******************************/
/* Gra ASCII "Przygody kloca" */
/* © 2016 Kamil Poniatowski   */
/******************************/

#ifndef game_h
#define game_h

int playGame( int world, Windows w );
void print_map( int map, Player *player, Monster *monsters, Monster_list **fmonster_on_map, Box_list **fbox, WINDOW *win, WINDOW *twin );
void add_to_item_list( Item_list **fitem, Item item );
void add_box_to_list( Box_list **fbox, int y, int x, Item_list *fitem );
void prplayer_xy( WINDOW *win, int y, int x );
void prfch_xy( WINDOW *win, char ch, int y, int x );
void prmon_xy( WINDOW *win, Monster *monsters, int id, int y, int x );
void prlife( WINDOW *win, Player player, Monster_list *monster );
void load_monsters( Monster **monsters );
void load_weapons( Weapon **weapons );
void load_armors( Armor **armors );
void add_monster( Monster *monsters, Monster_list **fmonster, int id, int uniId, int y, int x );
void add_monster_saved( Monster_list mToAdd, Monster_list **fmonster, int uniId );
void delete_dead_monster( WINDOW *win, Monster_list **pointer, Monster_list **fmonster );
void clear_list( Monster_list **monster );
void clear_list_box( Box_list **fbox );
Monster_list *checkIfMonsterNearPlayer( Player *player, Monster_list *fmonster );
void save_game( WINDOW *win, int world, Player player, Monster_list *fmonster, Monster *monsters );
int load_player( Player *player );
void init_player( Player *player );
int load_saved_game( Player *player, Monster *monsters, Monster_list **fmonster, WINDOW *win );
void check_player_exp( Player *player );
void print_player_info( WINDOW *win, Player player, Armor *armors, Weapon *weapons );
void open_box( Player *player, int y, int x, Box_list **fbox, WINDOW *win, int world );
Box_list *find_box( Box_list **fbox, int y, int x );
void load_maxworlds( int *max_worlds );

#endif
