#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#ifndef __CHESS_H__

#define __CHESS_H_

#define TAILLE_LIGNE 8
#define TAILLE_ECHEQUIER 64

#define VIDE 0
#define BLANC 1
#define NOIR 2
#define U64 uint64_t
#define HUMAIN 0
#define IA 1

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

#define MAX_TABLE_SIZE 1000000 //1 million
#define NB_LIGNES 489703
#define RAND_64 ((U64)rand() |       \
                 (U64)rand() << 15 | \
                 (U64)rand() << 30 | \
                 (U64)rand() << 45 | \
                 ((U64)rand() & 0xf) << 60)

typedef struct liste
{
    uint_fast8_t valeur;
    struct liste *next;
} liste;

typedef struct nouvelle_partie
{
    int joueur_blanc;
    int joueur_noir;
    int profondeur_blanc;
    int profondeur_noir;
    int quitter;
    int debug;
} nouvelle_partie;

typedef struct FEN
{
    int tour;                   // BLANC OU NOIR (1 ou 2)
    int nb_pcs_w;               //NOMBRE DE PIECES BLANCHES
    int nb_pcs_b;               //NOMBRE DE PIECES NOIRES
    int nb_tours;               //incrémenté a chaque tour
    int half_move;              // incrémenté a chaque tour, réinitialisé après une capture
    int_fast8_t full_move;      // incrémenté a chaque tour des noirs
    int echec;                  // -1 par défaut, prend la couleur du roi en échec
    int echec_et_mat;           // -1 par défaut, prend la couleur du perdant (NOIR ou BLANC)
    int_fast8_t capture;        // 0 par défaut, 1 si le dernier move etait une capture
    int_fast8_t endgame;        // 0 par défaut, 1 quand on est en endgame
} FEN;
typedef struct best_move
{
    uint_fast8_t piece;
    int_fast8_t move;
    int score;
} best_move;

typedef struct Entry
{
    U64 posKey; // position
    int score;  // score associé
} Entry;

typedef struct Hash_table
{
    Entry **entries; // tableau d'entries (poskey + move)
    int nb_entries;
} Hash_table;

extern best_move *return_minimax;
extern FEN *fen;
extern Hash_table *hashtable;
extern uint_fast8_t echequier[TAILLE_ECHEQUIER];
extern U64 PieceKeys[14][64];
extern U64 SideKey;
extern double total_search;

// FONCTIONS LISTE:
liste *creation_maillon(int_fast8_t n);

void affichage_liste(liste *l);

liste *ajout_tete(liste *l, liste *e);

int_fast8_t recherche(liste *l, int_fast8_t e);

liste *suppression_valeur(liste *l, int_fast8_t m);

void liberation(liste *l);

// FONCTIONS INFORMATIVES:

int_fast8_t get_pos(int_fast8_t ligne, int_fast8_t colonne);

int_fast8_t get_ligne(int_fast8_t position);

int_fast8_t get_colonne(int_fast8_t position);

int_fast8_t compter_pieces(int_fast8_t couleur, uint_fast8_t *plateau);

int_fast8_t get_color(uint_fast8_t piece);

int_fast8_t vide(int_fast8_t a, int_fast8_t b, uint_fast8_t *plateau);

void afficher_liste_pieces(int_fast8_t couleur, uint_fast8_t *plateau);

// FONCTIONS DE BASE DU JEU:
void affichage_echequier();

int affichage_echequier_fichier();

void affichage_echequier_alt();

nouvelle_partie select_mode();

void initialiser_jeu();

void initialiser_fen();

void init_return_minimax();

int_fast8_t print_piece(uint_fast8_t position);

void print_name(uint_fast8_t piece);

void print_color(uint_fast8_t piece);

int select_piece(int_fast8_t tour, uint_fast8_t *plateau);

void update_fen(FEN *fen);

int_fast8_t verifier_echec(uint_fast8_t *plateau);

int_fast8_t verifier_echec_fast(int_fast8_t couleur, uint_fast8_t *plateau);

int_fast8_t check_endgame(uint_fast8_t *plateau);

// FONCTIONS DE FORMATTAGE DE TABLEAUX:

uint_fast8_t *copie_echequier(uint_fast8_t *plateau, uint_fast8_t *tab);

// FONCTIONS DE RECUPERATION DE MOVES:

liste *get_legal_pion_blanc(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_pion_noir(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_cavalier(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_tour(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_fou(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_roi(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_reine(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_any(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *get_legal_all(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *retirer_echec(int_fast8_t position, liste *moves, uint_fast8_t *plateau);

liste *liste_moves(int_fast8_t couleur, liste *liste_pieces, uint_fast8_t *plateau);

int_fast8_t echec_et_mat(int_fast8_t couleur, uint_fast8_t *plateau);

int_fast8_t choisir_move(int_fast8_t position, uint_fast8_t *plateau);

void effectuer_move(int_fast8_t position_piece, int_fast8_t position_move, uint_fast8_t *plateau);

void ia_move(int_fast8_t profondeur, int_fast8_t couleur, int debug, uint_fast8_t *plateau);

void player_move(int_fast8_t couleur, uint_fast8_t *plateau);

/* ************************
// FICHIER IA:
***************************/

// FONCITONS SCORE:

int get_valeur_materielle(int_fast8_t position, uint_fast8_t *plateau);

int get_valeur_materielle_totale(int_fast8_t couleur, uint_fast8_t *plateau);

int get_bonus_placements(uint_fast8_t piece, int_fast8_t position);

int get_bonus_placements_total(int_fast8_t couleur, uint_fast8_t *plateau);

int get_score_couleur(int_fast8_t couleur, uint_fast8_t *plateau);

int get_score(uint_fast8_t *plateau);

// MINIMAX:
int_fast8_t get_minimizer(int_fast8_t maximizer);

int_fast8_t get_couleur_ennemie(int_fast8_t couleur);

int get_max(int a, int b);

int get_min(int a, int b);

int minimax(int_fast8_t couleur, int_fast8_t maximizer, uint_fast8_t *plateau, int_fast8_t profondeur, int alpha, int beta);

/* ************************
// FICHIER HASHTABLE:
***************************/

void init_hashkeys();

U64 generate_posKey(uint_fast8_t *plateau, int_fast8_t tour);

Hash_table *init_hashtable(Hash_table *hashtable);

void add_entry(Hash_table *hashtable, U64 posKey, int score);

int search_table(Hash_table *hashtable, U64 posKey);

void liberation_hashtable(Hash_table * hashtable);

void fill_from_file(Hash_table * hashtable);

int minimax_ht(int_fast8_t couleur, int_fast8_t maximizer, uint_fast8_t *plateau, int_fast8_t profondeur, int alpha, int beta, FILE * fp);

#endif