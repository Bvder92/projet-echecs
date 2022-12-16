#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned char echequier[64];
FEN fen;

int main()
{
    char position, move;
    int i = 0;
    char ligne, colonne;
    char tmp = 0;
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;
    unsigned char *plateau_tmp = (unsigned char *)malloc(sizeof(unsigned char)*TAILLE_ECHEQUIER);

    initialiser_jeu();
    fen = initialiser_fen(fen);
    while (fen.echec_et_mat == -1)
    {
        affichage_echequier();
        printf("SCORE: %d\n", get_score(echequier));

        if (fen.tour == BLANC)
        {
            printf("\nTour des Blancs:\n");
        }
        else
        {
            printf("\nTour des Noirs\n");
            plateau_tmp = copie_echequier(echequier, plateau_tmp);
            //test(echequier);
            printf("Meilleur score possible: %d", minimax(NOIR, 1, plateau_tmp, 3));
            printf("\n");
        }

        do
        {
            tmp = 0;
            if (fen.tour == NOIR)
            {
                liste_pieces = liste_moves(NOIR, liste_pieces, echequier);
                printf("\nSelectionner une piece Noire:\n\tColonne: ");
            }
            else
            {
                liste_pieces = liste_moves(BLANC, liste_pieces, echequier);
                printf("\nSelectionner une piece Blanche:\n\tColonne: ");
            }
            scanf("%d", &colonne);
            printf("\n\tLigne: ");
            scanf("%d", &ligne);

            position = (ligne * 8) + colonne;

            if (recherche(liste_pieces, position) == 0) // recherche(liste_pieces, position)
            {
                printf("\nCette piece ne peut pas bouger !");
            }
            else
            {
                tmp = 1;
            }
        } while (tmp == 0);

        move = choisir_move(position, echequier);
        effectuer_move(position, move, echequier);

        // free(plateau_tmp);
        fen = update_fen(fen);
    }
    affichage_echequier();
    if (fen.echec == BLANC)
    {
        printf("\nECHEC ROI BLANC!\n");
    }
    else
    {
        printf("\nECHEC ROI NOIR!\n");
    }
    return 0;
}