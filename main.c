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
    int maximizer;

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

        if (fen.tour == 1){
            printf("\nBest score: %d\n", minimaxx(position, 1, 2));
        }
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
        printf("\nSCORE NOIR: %d\n", get_score(0, echequier));
        //printf("SCORE NOIR: %d\n", get_score(1, echequier));

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