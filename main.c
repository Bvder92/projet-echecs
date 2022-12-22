#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned char echequier[TAILLE_ECHEQUIER];
FEN fen;
best_move return_minimax;

int main()
{
    char move;
    int i = 0, position;
    char ligne, colonne;
    char tmp = 0;
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;

    initialiser_jeu();
    fen = initialiser_fen(fen);
    while (fen.echec_et_mat == -1)
    {
        affichage_echequier_alt();
        printf("SCORE: %d\n", get_score(echequier));

        if (fen.tour == BLANC)
        {
            //position = select_piece(fen.tour, echequier);
            //move = choisir_move(position, echequier);
            //effectuer_move(position, move, echequier);
            printf("\nBLANC");
            ia_move(1, BLANC, echequier);
        }
        else if (fen.tour == NOIR)
        {
            printf("NOIR");
            ia_move(1, NOIR, echequier);
        }
        else
        {
            printf("\nFin du jeu, erreur fen.tour");
            break;
        }

        fen = update_fen(fen);
    }
    affichage_echequier_alt();
    if (fen.echec == BLANC)
    {
        printf("\nECHEC ROI BLANC!\n");
    }
    else if (fen.echec == NOIR)
    {
        printf("\nECHEC ROI NOIR!\n");
    }
    else {
        printf("\nFin erreur");
    }
    return 0;
}