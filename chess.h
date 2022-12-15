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
#define VALEUR_CAVALIER 30
#define VALEUR_FOU 30
#define VALEUR_TOUR 50
#define VALEUR_REINE 90
#define VALEUR_ROI 900

struct liste
{
    unsigned char valeur;
    struct liste *next;
};
typedef struct liste liste;

struct FEN
{
    char tour;         // BLANC OU NOIR (1 ou 2)
    char half_move;    // incrémenté a chaque tour
    char full_move;    // incrémenté a chaque tour des noirs
    char echec;        // -1 par défaut, prend la couleur du roi en échec
    char echec_et_mat; // -1 par défaut, prend la couleur du perdant (NOIR ou BLANC)
    int *castlew;
    int *castleb; // tableaux indiquant si on peut castle
};
typedef struct FEN FEN;

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

char vide(char a, char b, char position, unsigned char *plateau);

// FONCTIONS DE BASE DU JEU:
void affichage_echequier();

void initialiser_jeu();

FEN initialiser_fen(FEN fen);

char print_piece(unsigned char position);

void print_name(unsigned char piece);

void print_color(unsigned char piece);

char select_piece(char tour, unsigned char *plateau);

FEN update_fen(FEN fen);

char verifier_echec(unsigned char *plateau);

// FONCTIONS DE FORMATTAGE DE TABLEAUX:
char *copie_echequier(unsigned char *plateau, char *tab);

int *retirer_impossible(int *tab, int taille);

// FONCTIONS DE RECUPERATION DE MOVES:

liste *get_legal_pion_blanc(char position, liste *moves, unsigned char *plateau);

liste *get_legal_pion_noir(char position, liste *moves, unsigned char *plateau);

liste *get_legal_cavalier(char position, liste *moves, unsigned char *plateau);

liste *get_legal_tour(char position, liste *moves, unsigned char *plateau);

liste *get_legal_fou(char position, liste *moves, unsigned char *plateau);

liste *get_legal_roi(char position, liste *moves, unsigned char *plateau);

liste *get_legal_reine(char position, liste *moves, unsigned char *plateau);

liste *get_legal_any(char position, liste *moves, unsigned char *plateau);

liste *retirer_echec(char position, liste *moves, unsigned char *plateau);

liste *liste_moves(char couleur, liste *liste_pieces, unsigned char *plateau);

char echec_et_mat(char couleur, unsigned char * plateau);

char bouger(char position, unsigned char *plateau);

/* ************************
// FICHIER IA:
***************************/

// FONCITONS SCORE:

int get_valeur(char position, unsigned char *plateau);

int get_valeur_total(char couleur, unsigned char *plateau);

int get_score(unsigned char *plateau);

// MINIMAX:
int get_minimizer(char maximizer);

int get_couleur_ennemie(char couleur);

int get_max(int a, int b);

int get_min(int a, int b);

#endif