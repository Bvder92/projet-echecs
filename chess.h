#ifndef __CHESS_H__
#define __CHESS_H__

#define TAILLE_LIGNE 8
#define MAX 64

#define VIDE 0
#define PION 1
#define CAVALIER  2
#define FOU 3
#define TOUR 4
#define REINE 5
#define ROI 6
#define NOIR 128

enum pieces {empty, pawn, knight, bishop, rook, queen, king};

extern int echequier[MAX];

void init_echequier();

void placer_pion(int ligne, int colonne);

int get_pos(int ligne, int colonne);

void affichage_tableau(char * tab, int taille);
   
void affichage_echequier();

void initialiser_jeu();

void initialiser_debug();

int get_ligne(int position);

int get_colonne(int position);

void print_name(int piece);

void print_color(int position);

int * retirer_impossible(int * tab, int taille);

void bouger_pion(int position);
    
int * get_legal_cavalier(int position, int * array);

int * get_legal_tour(int position, int * moves, int taille);

int * get_legal_fou(int position, int * moves, int taille);

int * get_legal_roi(int position, int * moves);

int * get_legal_reine(int position, int * moves, int taille);

int bouger(int position);

#endif
