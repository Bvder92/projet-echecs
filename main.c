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

    initialiser_jeu(fen);
    /*fen.tour = 1;
    while (1)
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
        printf("\n\t~~~~position retournee~~~~");
        move = bouger(position);
        
        echequier[move] = echequier[position];
        echequier[position] = VIDE;
        //fen = update_fen(fen);
    }*/
    print_name(6);
    print_color(6);
    return 0;
}
