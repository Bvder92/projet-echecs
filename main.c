#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int echequier[64];
FEN fen;

int main()
{
    int position, rep = 0;
    int move;

    initialiser_jeu();
    fen = initialiser_fen(fen);
    while (fen.echec_et_mat == -1)
    {
        affichage_echequier();
        if (fen.tour == 0)
        {
            printf("\nTour des Blancs:\n");
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
        if (echequier[position] == ROI) //si la piece qu'on bouge est un roi qui a jamais bougé
        {
            if (move == 62) //62 correspond au castle côté roi blanc -> on bouge la tour droite
            {
                echequier[61] = echequier[63];
                echequier[63] = VIDE;
            }
            if (move == 58) //58 correspond au castle côté reine blanche -> on bouge la tour gauche
            {
                echequier[59] = echequier[56];
                echequier[56] = VIDE;
            }
        }
        if (echequier[position] == ROI + NOIR) //même chose pour côté noir
        {
            if (move == position + 2)
            {
                echequier[5] = echequier[7];
                echequier[7] = VIDE;
            }
            if (move == position - 2)
            {
                echequier[3] = echequier[0];
                echequier[0] = VIDE;
            }
        }

        /* **Incrémentation des pions et rois de SPECIAL** */
        if (echequier[position] == ROI || echequier[position] == ROI + NOIR)
        {
            echequier[position] += SPECIAL;
        }

        if (echequier[position] == PION || echequier[position] == PION + NOIR)
        {
            echequier[position] += SPECIAL;
        }


        /* **On effectue le mouvement et update le fen** */
        echequier[move] = echequier[position];
        echequier[position] = VIDE;

        fen = update_fen(fen);
        printf("\nSCORE BLANC: %d\n", get_score(0, echequier));
        printf("SCORE NOIR: %d\n", get_score(1, echequier));

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