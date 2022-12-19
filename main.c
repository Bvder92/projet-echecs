#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

unsigned char echequier[TAILLE_ECHEQUIER];
FEN fen;
best_move return_minimax;

int main()
{
    char position, move;
    int i = 0;
    char ligne, colonne;
    char tmp = 0;
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;

    initialiser_jeu();
    fen = initialiser_fen(fen);
    while (fen.echec_et_mat == -1)
    {
        affichage_echequier();
        printf("SCORE: %d\n", get_score(echequier));

        if (fen.tour == BLANC)
        {
            printf("\nTour des Blancs:\n");
            liste_pieces = liste_moves(BLANC, liste_pieces, echequier);
            printf("Liste des pieces qui peuvent bouger: ");
            affichage_liste(liste_pieces);
            minimax(BLANC, 0, echequier, 4);
            printf("\n\t***MEILLEUR COUP POSSIBLE***");
            printf("\nPiece: (%d,%d), ", get_colonne(return_minimax.piece), get_ligne(return_minimax.piece));
            printf("Move: (%d,%d), ", get_colonne(return_minimax.move), get_ligne(return_minimax.move));
            printf("Score: %d\n", return_minimax.score);
        }
        else
        {
            printf("\nTour des Noirs\n");
            liste_pieces = liste_moves(NOIR, liste_pieces, echequier);
            printf("Liste des pieces qui peuvent bouger: ");
            affichage_liste(liste_pieces);
            minimax(NOIR, 1, echequier, 4);
            printf("\n\t***MEILLEUR COUP POSSIBLE***");
            printf("\nPiece: (%d,%d), ", get_colonne(return_minimax.piece), get_ligne(return_minimax.piece));
            printf("Move: (%d,%d), ", get_colonne(return_minimax.move), get_ligne(return_minimax.move));
            printf("Score: %d\n", return_minimax.score);
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