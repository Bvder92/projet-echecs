#ifndef __CHESS_H__

#define __CHESS_H_

#define TAILLE_LIGNE 8
#define TAILLE_ECHEQUIER 64

#define VIDE 0
#define BLANC 1
#define NOIR 2

#define PION 1
#define CAVALIER 2
#define FOU 3
#define TOUR 4
#define REINE 5
#define ROI 6
#define PIECE_NOIRE 128
#define PIECE_SPECIAL 100

#define VALEUR_PION 10
#define VALEUR_CAVALIER 32
#define VALEUR_FOU 33
#define VALEUR_TOUR 50
#define VALEUR_REINE 90
#define VALEUR_ROI 2000

struct liste
{
    unsigned char valeur;
    struct liste *next;
};
typedef struct liste liste;

struct FEN
{
    int tour;         // BLANC OU NOIR (1 ou 2)
    int half_move;    // incrémenté a chaque tour
    char full_move;    // incrémenté a chaque tour des noirs
    int echec;        // -1 par défaut, prend la couleur du roi en échec
    int echec_et_mat; // -1 par défaut, prend la couleur du perdant (NOIR ou BLANC)
    char capture;      // 0 par défaut, 1 si le dernier move etait une capture
    char endgame;      // 0 par défaut, 1 quand on est en endgame
    int promos;
};
typedef struct FEN FEN;

struct best_move
{
    unsigned char piece;
    char move;
    int score;
};
typedef struct best_move best_move;

extern best_move return_minimax;

extern FEN fen;

extern unsigned char echequier[TAILLE_ECHEQUIER];

// FONCTIONS LISTE:
liste *creation_maillon(char n);

char liste_vide(liste *l);

void affichage_liste(liste *l);

liste *ajout_tete(liste *l, liste *e);

liste *supprimer_tete(liste *l);

liste *ajout_queue(liste *l, liste *m);

liste *suppression_queue(liste *l);

char recherche(liste * l, char e);

liste *suppression_valeur(liste *l, char m);

void liberation(liste *l);

// FONCTIONS INFORMATIVES:

char get_pos(char ligne, char colonne);

char get_ligne(char position);

char get_colonne(char position);

char compter_pieces(char couleur, unsigned char *plateau);

char get_color(unsigned char piece);

char vide(char a, char b, unsigned char *plateau);

void afficher_liste_pieces(char couleur, unsigned char * plateau);

// FONCTIONS DE BASE DU JEU:
void affichage_echequier();

int affichage_echequier_fichier();

void affichage_echequier_alt();

void initialiser_jeu();

FEN initialiser_fen(FEN fen);

char print_piece(unsigned char position);

void print_name(unsigned char piece);

void print_color(unsigned char piece);

int select_piece(char tour, unsigned char *plateau);

FEN update_fen(FEN fen);

char verifier_echec(unsigned char *plateau);

char verifier_echec_couleur(char couleur, unsigned char *plateau);

char check_endgame(unsigned char * plateau);

// FONCTIONS DE FORMATTAGE DE TABLEAUX:

unsigned char *copie_echequier(unsigned char *plateau, unsigned char *tab);

// FONCTIONS DE RECUPERATION DE MOVES:

liste *get_legal_pion_blanc(char position, liste *moves, unsigned char *plateau);

liste *get_legal_pion_noir(char position, liste *moves, unsigned char *plateau);

liste *get_legal_cavalier(char position, liste *moves, unsigned char *plateau);

liste *get_legal_tour(char position, liste *moves, unsigned char *plateau);

liste *get_legal_fou(char position, liste *moves, unsigned char *plateau);

liste *get_legal_roi(char position, liste *moves, unsigned char *plateau);

liste *get_legal_reine(char position, liste *moves, unsigned char *plateau);

liste *get_legal_any(char position, liste *moves, unsigned char *plateau);

liste * get_legal_all(char position, liste * moves, unsigned char * plateau);

liste *retirer_echec(char position, liste *moves, unsigned char *plateau);

liste *liste_moves(char couleur, liste *liste_pieces, unsigned char *plateau);

char echec_et_mat(char couleur, unsigned char * plateau);

char choisir_move(char position, unsigned char *plateau);

void effectuer_move(char position_piece, char position_move, unsigned char *plateau);

void ia_move(char profondeur, char couleur, unsigned char * plateau);

void promotion_ia(char position, unsigned char nouvelle_piece, unsigned char *plateau);

void promotion_user(char position, char piece, unsigned char *plateau);

/* ************************
// FICHIER IA:
***************************/

// FONCITONS SCORE:

int get_valeur_materielle(char position, unsigned char *plateau);

int get_valeur_materielle_totale(char couleur, unsigned char *plateau);

int get_bonus_placements(unsigned char piece, char position);

int get_bonus_placements_total(char couleur, unsigned char * plateau);

int get_score(unsigned char *plateau);

// MINIMAX:
char get_minimizer(char maximizer);

char get_couleur_ennemie(char couleur);

int get_max(int a, int b);

int get_min(int a, int b);

int minimax(char couleur, char maximizer, unsigned char *plateau, char profondeur, int alpha, int beta);

int minimax_old(char couleur, char maximizer, unsigned char *plateau, char profondeur);

void test();

#endif