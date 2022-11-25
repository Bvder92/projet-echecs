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

struct FEN{
    int tab[MAX]; //echequier
    int tour; //0 si blanc, 1 si noir (un int c'est mieux qu'un booléen)
    int *castlew; 
    int *castleb; //coté reine alors "q" coté roi "k" sinon -
    int en_passant; //coordonées du tableau pour en_passant
    int half_move; 
    int full_move;
    int echec_blanc; //pour l'instant c'est juste du 0/1 mais si besoin on peut compter toutes les pieces
    int echec_noir; //0 par défaut
    int echec_et_mat; //-1 defaut, prend la couleur du perdant
}; 
typedef struct FEN FEN; 

extern int echequier[MAX];

int get_pos(int ligne, int colonne);

FEN initialiser_fen(FEN fen);

char int_to_piece(int position);
   
void affichage_echequier();

void initialiser_jeu();

void initialiser_debug();

int get_ligne(int position);

int get_colonne(int position);

int compter_blanc();

int compter_noir();

void print_name(int piece);

void print_color(int position);

int get_color(int position);

void debug_mode();

int select_piece();

int * retirer_impossible(int * tab, int taille);

int * get_legal_pion_blanc(int position, int * moves);

int * get_legal_pion_noir(int position, int * moves);
    
int * get_legal_cavalier(int position, int * array);

int * get_legal_tour(int position, int * moves, int taille);

int * get_legal_fou(int position, int * moves, int taille);

int * get_legal_roi(int position, int * moves);

int * get_legal_reine(int position, int * moves, int taille);

int get_taille_moves(int position);

int * recuperer_moves(int position, int taille);

int bouger(int position);

FEN verifier_echec(FEN fen);

FEN update_fen(FEN fen);


#endif
