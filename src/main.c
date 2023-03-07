#include "../include/chess.h"

uint_fast8_t echequier[TAILLE_ECHEQUIER];
U64 PieceKeys[14][64];
U64 SideKey;
FEN *fen;
best_move *return_minimax;
Hash_table *hashtable;
double total_search;

int main()
{
    nouvelle_partie partie = select_mode();

    if (partie.quitter == 1)
    {
        return 0;
    }

    total_search = 0.0;
    initialiser_jeu();
    affichage_echequier();
    //fill_from_file(hashtable);
    printf("\n");
    while (fen->echec_et_mat == -1 && fen->half_move < 50)
    {
        if (partie.debug == 1)
        {
            printf("\nSCORE: %d", get_score(echequier));
            printf("\nHalf-moves: %d, Total: %d", fen->half_move, fen->nb_tours);
            printf("\nEndgame: %d\n", fen->endgame);
            printf("\n\nTemps de recherche total: %f", total_search);
            printf("\nHASH: %llu", generate_posKey(echequier, fen->tour));
            printf("\nHashtable entries: %d\n", hashtable->nb_entries);
        }

        if (fen->tour == BLANC)
        {
            if (partie.joueur_blanc == HUMAIN)
            {
                player_move(BLANC, echequier);
            }
            else
            {
                ia_move(partie.profondeur_blanc, BLANC, partie.debug, echequier);
            }

            if (compter_pieces(NOIR, echequier) != fen->nb_pcs_b)
            {
                fen->capture = 1;
            }
        }
        else
        {
            if (partie.joueur_noir == HUMAIN)
            {
                player_move(NOIR, echequier);
            }
            else
            {
                ia_move(partie.profondeur_noir, NOIR, partie.debug, echequier);
            }

            if (compter_pieces(BLANC, echequier) != fen->nb_pcs_w)
            {
                fen->capture = 1;
            }
        }

        update_fen(fen);
        affichage_echequier();
    }

    if (fen->echec_et_mat == BLANC)
    {
        printf("\nECHEC ET MAT, VICTOIRE DES NOIRS! en %d tours\n", fen->nb_tours);
    }
    else if (fen->echec_et_mat == NOIR)
    {
        printf("\nECHEC ET MAT, VICTOIRE DES BLANCS! en %d tours\n", fen->nb_tours);
    }
    else if (fen->half_move == 50)
    {
        printf("\nEgalite: 50 moves a la suite sans captures...\nNombre de tours: %d", fen->nb_tours);
    }
    if (partie.debug == 1 || partie.debug == 2)
    {
        printf("\nSCORE: %d", get_score(echequier));
        printf("\nEndgame: %d\n", fen->endgame);
        printf("\nHASH: %llu", generate_posKey(echequier, fen->tour));
        printf("\nHashtable entries: %d\n", hashtable->nb_entries);
        printf("\n\nTemps de recherche total: %f", total_search);
        afficher_liste_pieces(BLANC, echequier);
        afficher_liste_pieces(NOIR, echequier);
    }

    liberation_hashtable(hashtable);

    printf("\n\n\n\n");
    return 0;
}