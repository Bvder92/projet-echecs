#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned char echequier[TAILLE_ECHEQUIER];
U64 PieceKeys[14][64];
U64 SideKey;
FEN *fen;
best_move *return_minimax;
Hash_table *hashtable;
double total_search;

void init_return_minimax()
{
    return_minimax = (best_move *)malloc(sizeof(return_minimax));
    return_minimax->move = 0;
    return_minimax->piece = 0;
    return_minimax->score = 0;
}

int main()
{
    total_search = 0.0;
    char move;
    char nb_pieces_blanches;
    char nb_pieces_noires;
    int i = 0, position;
    char ligne, colonne;
    int nbtours = 0;
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;

    initialiser_jeu();
    initialiser_fen();
    init_return_minimax();
    InitHashKeys();
    init_hashtable(hashtable);
    affichage_echequier();
    while (fen->echec_et_mat == -1 && fen->half_move < 50)
    {
        nb_pieces_blanches = compter_pieces(BLANC, echequier);
        nb_pieces_noires = compter_pieces(NOIR, echequier);
        printf("\nSCORE: %d", get_score(echequier));
        printf("\nHASH: %llu", GeneratePosKey(echequier, fen->tour));
        printf("\nHalf-moves: %d", fen->half_move);
        printf("\nEndgame: %d\n", fen->endgame);

        if (fen->tour == BLANC)
        {
            // printf("Score materiel: %d, placements: %d\n", get_valeur_materielle_totale(BLANC, echequier), get_bonus_placements_total(BLANC, echequier));
             ia_move(4, BLANC, echequier);
            //liste_pieces = liste_moves(BLANC, liste_pieces, echequier);
            //affichage_liste(liste_pieces);
            //player_move(BLANC, echequier);

            if (compter_pieces(NOIR, echequier) != nb_pieces_noires)
            {
                fen->capture = 1;
            }
        }
        else if (fen->tour == NOIR)
        {
             ia_move(4, NOIR, echequier);
            //liste_pieces = liste_moves(NOIR, liste_pieces, echequier);
            //affichage_liste(liste_pieces);
            //player_move(NOIR, echequier);
            // printf("Score materiel: %d, placements: %d\n", get_valeur_materielle_totale(NOIR, echequier), get_bonus_placements_total(NOIR, echequier));

            if (compter_pieces(BLANC, echequier) != nb_pieces_blanches)
            {
                fen->capture = 1;
            }
        }

        else
        {
            printf("\nFin du jeu, erreur fen->tour");
            break;
        }

        update_fen(fen);
        affichage_echequier();

        // affichage_echequier_fichier();
        nbtours++;
    }

    if (fen->echec_et_mat == BLANC)
    {
        printf("\nECHEC ET MAT, VICTOIRE DES NOIRS! en %d tours\n", nbtours);
    }
    else if (fen->echec_et_mat == NOIR)
    {
        printf("\nECHEC ET MAT, VICTOIRE DES BLANCS! en %d tours\n", nbtours);
    }
    else if (fen->half_move == 50)
    {
        printf("\nEgalite: 50 moves a la suite sans captures\nNombre de tours: %d", nbtours);
    }
    afficher_liste_pieces(BLANC, echequier);
    afficher_liste_pieces(NOIR, echequier);
    printf("\n\nTemps de recherche total: %f", total_search);
    printf("\n\n\n\n");
    return 0;
}