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
        move = bouger(position, fen);
        printf("\nMove = %d\n", move);
        
        if (echequier[position] == ROI){
            if (move == 62){
                echequier[61] = echequier[63];
                echequier[63] = VIDE;
            }
            if (move == 58){
                echequier[59] = echequier[56];
                echequier[56] = VIDE;
            }
        }

        if (echequier[position] == ROI+NOIR){
            if (move == position+2){
                echequier[5] = echequier[7];
                echequier[7] = VIDE;
            }
            if (move == position - 2){
                echequier[3] = echequier[0];
                echequier[0] = VIDE;
            }
        }

        echequier[move] = echequier[position];
        echequier[position] = VIDE;

        fen = update_fen(fen);
        if (fen.castleb != NULL){
            printf("\ncastleb: %d %d", fen.castleb[0], fen.castleb[1]);
        }
        if (fen.castlew != NULL){
            printf("\ncastlew: %d %d", fen.castlew[0], fen.castlew[1]);
        }
        printf("\n");
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