#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int echequier[64];
FEN fen;

int main()
{
    /*int position, rep = 0;
    int move;

    initialiser_jeu(fen);
    while ((fen.echec_blanc == 0) && (fen.echec_noir == 0))
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
        position = select_piece(fen.tour);
        move = bouger(position);
        
        echequier[move] = echequier[position];
        echequier[position] = VIDE;
        fen = update_fen(fen);
        printf("\n**************UPDATE REUSSIE****************\n");

    }

    printf("\n%d, %d", fen.echec_blanc, fen.echec_noir);

    printf("\nECHEC ROI ");
    if (fen.echec_blanc != 0){
        printf("BLANC");
    }
    else if (fen.echec_noir != 0){
        printf("NOIR");
    }
    else{
        printf("Les deux?????");
    }*/
    debug_mode();
    return 0;
}
