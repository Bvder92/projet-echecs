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
    int i = 0, position;
    char ligne, colonne;
    char tmp = 0;
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;

    initialiser_jeu();
    fen = initialiser_fen(fen);
    affichage_echequier();
    while (fen.echec_et_mat == -1)
    {
        printf("SCORE: %d\n", get_score(echequier));

        if (fen.tour == BLANC)
        {   
            /*
            position = select_piece(fen.tour, echequier);
            move = choisir_move(position, echequier);
            effectuer_move(position, move, echequier);
            */
            printf("\nBLANC");
            ia_move(4, BLANC, echequier);
        }
        else if (fen.tour == NOIR)
        {
            //printf("\nNOIR\n");
            ia_move(4, NOIR, echequier);
        }
        else
        {
            printf("\nFin du jeu, erreur fen.tour");
            break;
        }

        fen = update_fen(fen);
        printf("\n%d\n", fen.half_move);
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
    else
    {
        printf("\nFin erreur");
    }
    return 0;
}