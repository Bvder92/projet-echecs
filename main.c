#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned char echequier[TAILLE_ECHEQUIER];
FEN fen;
best_move return_minimax;
int file = -1;

int main()
{
    char move;
    char nb_pieces_blanches;
    char nb_pieces_noires;
    int i = 0, position;
    char ligne, colonne;
    char tmp = 0;
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;

    initialiser_jeu();
    fen = initialiser_fen(fen);
    affichage_echequier();
    // test();
    while (fen.echec_et_mat == -1 && fen.half_move < 50)
    {
        nb_pieces_blanches = compter_pieces(BLANC, echequier);
        nb_pieces_noires = compter_pieces(NOIR, echequier);
        printf("SCORE: %d\n", get_score(echequier));
        //test();

        if (fen.tour == BLANC)
        {

            printf("\nBLANC\n");
            // printf("Score materiel: %d, placements: %d\n", get_valeur_materielle_totale(BLANC, echequier), get_bonus_placements_total(BLANC, echequier));
            position = select_piece(fen.tour, echequier);
            move = choisir_move(position, echequier);
            effectuer_move(position, move, echequier);
            //ia_move(4, BLANC, echequier);

            if (compter_pieces(NOIR, echequier) != nb_pieces_noires)
            {
                fen.capture = 1;
            }
        }
        else if (fen.tour == NOIR)
        {
            printf("\nNOIR\n");
            ia_move(4, NOIR, echequier);
            // printf("Score materiel: %d, placements: %d\n", get_valeur_materielle_totale(NOIR, echequier), get_bonus_placements_total(NOIR, echequier));
            //position = select_piece(fen.tour, echequier);
            //move = choisir_move(position, echequier);
            //effectuer_move(position, move, echequier);

            if (compter_pieces(BLANC, echequier) != nb_pieces_blanches)
            {
                fen.capture = 1;
            }
        }

        else
        {
            printf("\nFin du jeu, erreur fen.tour");
            break;
        }

        fen = update_fen(fen);
        printf("\nfen.half_move: %d\n", fen.half_move);
        /*file = affichage_echequier_fichier();
        if (file == 0)
        {
            break;
        }
        if (file == -1)
        {
            fprintf(stderr, "\nerreur de batard\n");
        }*/
        affichage_echequier();
    }

    if (fen.echec == BLANC)
    {
        printf("\nECHEC ROI BLANC!\n");
    }
    else if (fen.echec == NOIR)
    {
        printf("\nECHEC ROI NOIR!\n");
    }
    else if (fen.half_move == 50)
    {
        printf("\nEgalite: 50 moves a la suite sans captures\n");
    }
    return 0;
}