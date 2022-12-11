#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

int echequier[64];
FEN fen;

int main()
{
    int position, rep = 0;
    int move;
    int maximizer;

    initialiser_jeu();
    fen = initialiser_fen(fen);
    while (fen.echec_et_mat == -1)
    {
        affichage_echequier();
        if (fen.tour == 0)
        {
            printf("\nTour des Blancs:\nMAXIMISATION DU SCORE BLANC: ");
            printf("%d\n", encore(1, 0, 1, echequier, INT_MIN, INT_MAX));
        }
        else
        {
            printf("\nTour des Noirs:\n");
        }

        do
        {
            position = select_piece_mieux(fen.tour);
            if (position == -1)
            {
                printf("\nCette piece ne peut pas bouger!\n");
            }
        } while (position == -1);

        move = bouger(position, fen);

        /* **Si le mouvement est un castle, on bouge la tour** */
        if (echequier[position] == ROI || echequier[position] == ROI+NOIR)//on bouge un roi pour la 1ere fois
        {
            bouger_tour_castle(position, move); //si le move est un castle, on bouge la tour au bon endroit
            echequier[position] += SPECIAL; // on vient de bouger un roi donc on incremente
        }

        /* **Incrémentation des pions et rois de SPECIAL** */
        if (echequier[position] == PION || echequier[position] == PION + NOIR)
        {
            echequier[position] += SPECIAL;
        }

        /* **On effectue le mouvement et update le fen** */
        echequier[move] = echequier[position];
        echequier[position] = VIDE;

        fen = update_fen(fen);
    }

    affichage_echequier();
    printf("\nECHEC ET MAT: VICTOIRE DES ");
    if (fen.echec_et_mat == 1)
    {
        printf("BLANCS\n");
    }
    else
    {
        printf("NOIRS\n");
    }
    return 0;
}