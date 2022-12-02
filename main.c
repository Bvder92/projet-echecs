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
        
        do{
            position = select_piece_mieux(fen.tour);
            if (position == -1){
                printf("\nCette piece ne peut pas bouger!\n");
            }
        } while(position == -1);
        move = bouger(position);
        
        echequier[move] = echequier[position];
        echequier[position] = VIDE;
        fen = update_fen(fen);
    }

    affichage_echequier();
    printf("\nECHEC ET MAT: VICTOIRE DES ");
    if (fen.echec_et_mat == 1){
        printf("BLANCS\n");
    }
    else{
        printf("NOIRS\n");
    }
    return 0;
}