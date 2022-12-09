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
#define SPECIAL 100

#define VALEUR_PION 10
#define VALEUR_CAVALIER 30
#define VALEUR_FOU 30
#define VALEUR_TOUR 50
#define VALEUR_REINE 90
#define VALEUR_ROI 900

struct FEN{
    int tab[MAX]; //echequier
    int tour; //0 si blanc, 1 si noir (un int c'est mieux qu'un booléen)
    int *castlew; 
    int *castleb; //coté reine alors "q" coté roi "k" sinon -
    int en_passant; //coordonées du tableau pour en_passant
    int half_move; 
    int full_move;
    int echec; //-1 de base, prend la couleur du perdant (1 si le roi noir est en echec)
    int echec_et_mat; //-1 defaut, prend la couleur du perdant
}; 
typedef struct FEN FEN; 

struct AI{
    int plateau_prev[MAX]; //echequier du tour precendent
    int maximizer; //couleur qui maximize
    int maximizer_score; // score de la couleur qui maximize
};
typedef struct AI AI;

extern int echequier[MAX];

int get_pos(int ligne, int colonne);

FEN initialiser_fen(FEN fen);

char int_to_piece(int position);
   
void affichage_echequier();

void initialiser_jeu();

void initialiser_debug();

int get_ligne(int position);

int get_colonne(int position);

int * copie_echequier(int * tab);

int compter_blanc();

int compter_noir();

void print_name(int piece);

void print_color(int position);

int get_color(int position, int * tab);

void debug_mode();

int select_piece(int tour);

int select_piece_mieux(int couleur);

int * retirer_impossible(int * tab, int taille);

int * get_legal_pion_blanc(int position, int * moves, int * tab);

int * get_legal_pion_noir(int position, int * moves, int * tab);
    
int * get_legal_cavalier(int position, int * moves, int * tab);

int * get_legal_tour(int position, int * moves, int taille, int * tab);

int * get_legal_fou(int position, int * moves, int taille, int * tab);

int * get_legal_roi(int position, int * moves, int * tab);

int * get_legal_reine(int position, int * moves, int taille, int * tab);

int get_taille_moves(int position, int * tab);

int * recuperer_moves(int position, int taille, int * tab);

int * retirer_echec(int position, int * moves);

int * liste_moves(int couleur, int * liste_pieces, int taille_liste);

int * get_moves_total(int * moves, int taille, int position, FEN fen);

int bouger(int position, FEN fen);

int verifier_echec(int * tab);

FEN update_fen(FEN fen);

int * castle(int position);

void bouger_tour_castle(int position, int move);

//minimax:

int get_valeur(int position, int * tab);

int get_valeur_total(int couleur, int *tab);

int get_score(int couleur, int * tab);

int get_score_move(int position, int move);

int * get_score_all_moves(int position, FEN fen);

int minimax(int position, int profondeur, int couleur_maximizer);

#endif
