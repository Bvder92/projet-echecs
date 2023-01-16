#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// const int PvSize = 0x100000 * 2;
uint_fast8_t get_piece_index(uint_fast8_t piece)
{
    uint_fast8_t finalpiece;
    if (get_color(piece) == NOIR)
    {
        if (piece == ROI + PIECE_NOIRE + PIECE_SPECIAL)
        {
            finalpiece = 14;
        }
        else
        {
            finalpiece = (piece - PIECE_NOIRE) + 7;
        }
        return finalpiece;
    }

    else if (piece == ROI + PIECE_SPECIAL)
    {
        return 7;
    }

    return piece;
}

// initialise les clés avec des nombres aléatoires
void init_hashkeys()
{
    int index = 0;
    int index2 = 0;

    for (index = 0; index < 14; ++index)
    {
        for (index2 = 0; index2 < 64; ++index2)
        {
            PieceKeys[index][index2] = RAND_64;
        }
    }
    SideKey = RAND_64;
}

// Genere un hash unique pour chaque échequier
U64 generate_posKey(uint_fast8_t *plateau, int_fast8_t tour)
{
    int i = 0;
    U64 FinalKey = 0;
    uint_fast8_t piece = VIDE;

    // pieces
    for (i = 0; i < TAILLE_ECHEQUIER; ++i)
    {
        piece = get_piece_index(plateau[i]); //change les valeurs des pièces pour [1-14]
        if (piece != VIDE)
        {
            if (piece >= PION && piece <= ROI + PIECE_NOIRE + PIECE_SPECIAL)
            {
                FinalKey ^= PieceKeys[piece][i];
            }
            else
            {
                fprintf(stderr, "\nERREUR KEY\n");
                return 0;
            }
        }
    }

    if (tour == BLANC)
    {
        FinalKey ^= SideKey;
    }

    return FinalKey;
}

Hash_table *init_hashtable(Hash_table *hashtable)
{
    hashtable = (Hash_table *)malloc(sizeof(hashtable));
    if (hashtable == NULL)
    {
        fprintf(stderr, "\nHASHTABLE NULL :(\n");
        return NULL;
    }

    hashtable->nb_entries = 0;
    hashtable->entries = (Entry **)malloc(sizeof(Entry *) * MAX_TABLE_SIZE); // tableau de pointeurs vers des entrées
    if (hashtable->entries == NULL)
    {
        fprintf(stderr, "\nENTRIES NULL\n");
        return NULL;
    }
    // allocation mémoire de chaque case du tableau:
    for (int i = 0; i < MAX_TABLE_SIZE; i++)
    {
        hashtable->entries[i] = (Entry *)malloc(sizeof(Entry));
        if (hashtable->entries[i] == NULL)
        {
            fprintf(stderr, "\nEntries[%d] NULL\n", i);
            return NULL;
        }
    }
    printf("\nHashTable initialisee avec %d entries\n", hashtable->nb_entries);
    return hashtable;
}

void add_entry(Hash_table *hashtable, U64 posKey, int score)
{
    if (hashtable->nb_entries == MAX_TABLE_SIZE - 50)
    {
        return;
    }
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    if (new_entry == NULL)
    {
        fprintf(stderr, "\nERREUR MALLOC NEWENTRY\n");
        return;
    }
    new_entry->posKey = posKey;
    new_entry->score = score;

    hashtable->entries[hashtable->nb_entries] = new_entry;
    hashtable->nb_entries++;
}

// parcours la hashtable, retourne un score si il existe une entrée avec la même clé,
// INT_MIN sinon
int search_table(Hash_table *hashtable, U64 posKey)
{
    for (int i = 0; i < hashtable->nb_entries; ++i)                                                         // tant que i est inférieur au nombre d'entrées dans la table
    {
        if (hashtable->entries[i]->posKey == posKey)
        {
            return hashtable->entries[i]->score;
        }
    }
    return INT_MIN; // la clé n'est pas dans la hashtable
}

void liberation_hashtable(Hash_table *hashtable)
{
    int i;
    for (i = 0; i < hashtable->nb_entries; i++)
    {

        free(hashtable->entries[i]);
    }
    free(hashtable);
}

void fill_from_file(Hash_table *hashtable)
{
    FILE *fp = fopen("aaa.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERR");
        return;
    }
    int i = 0, int_buf;
    U64 llu_buf;

    while (i < NB_LIGNES)
    {
        fscanf(fp, "%llu", &llu_buf);
        fseek(fp, 3, SEEK_CUR); // on décale le pointeur de 3 pour sauter " | "
        fscanf(fp, "%d", &int_buf);
        fseek(fp, 1, SEEK_CUR); // on décale le pointeur de 1 pour sauter le "\n"
        add_entry(hashtable, llu_buf, int_buf);
        i++;
    }
}

// couleur = couleur du joueur qui doit jouer, maximizer = es-ce qu'il veut maximizer ou minimizer son score
int minimax_ht(int_fast8_t couleur, int_fast8_t maximizer, uint_fast8_t *plateau, int_fast8_t profondeur, int alpha, int beta, FILE *fp)
{
    int_fast8_t couleur_ennemie = get_couleur_ennemie(couleur);
    if (profondeur == 0 || echec_et_mat(couleur, plateau) != -1)
    {
        // printf("profondeur = 0");
        return get_score(plateau);
    }

    int_fast8_t meilleur_move;
    uint_fast8_t meilleure_piece;

    int_fast8_t minimizer = get_minimizer(maximizer);

    int best_eval;
    int eval;
    int search;
    U64 posKey;

    int_fast8_t i = 0;
    uint_fast8_t *plateau_backup = (uint_fast8_t *)malloc(sizeof(uint_fast8_t) * TAILLE_ECHEQUIER);
    memcpy(plateau_backup, plateau, TAILLE_ECHEQUIER * sizeof(uint_fast8_t)); // utilisé pour reset l'echequier après qu'on ait testé un move

    if (maximizer == 1) // on cherche a maximizer le score
    {
        best_eval = INT_MIN; // int min car on veut maximizer -> on commence au minimum

        liste *liste_pieces = (liste *)malloc(sizeof(liste)); // liste des pieces pouvant bouger
        liste_pieces = NULL;
        liste_pieces = liste_moves(couleur, liste_pieces, plateau);

        while (liste_pieces != NULL) // POUR CHAQUE PIECE POUVANT BOUGER:
        {
            liste *moves = (liste *)malloc(sizeof(liste)); // liste des moves d'une piece
            moves = NULL;
            moves = get_legal_all(liste_pieces->valeur, moves, plateau);

            while (moves != NULL) // POUR CHAQUE MOVE DE LA PIECE:
            {
                // on effectue le move
                effectuer_move(liste_pieces->valeur, moves->valeur, plateau);

                posKey = generate_posKey(plateau, couleur);
                search = search_table(hashtable, posKey);
                if (search != INT_MIN) // si le move a déjà été évalué
                {
                    eval = search; // on récupère direct son éval
                }
                else
                {
                    eval = minimax_ht(couleur_ennemie, 0, plateau, profondeur - 1, alpha, beta, fp); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra minimizer)
                    add_entry(hashtable, posKey, eval);  //ancienne méthode, trop lente
                    //fprintf(fp, " %llu | %d\n", posKey, eval); //nouvelle méthode, on remplit un fichier avant l'éxécution puis on l'utilise en lecture
                }

                best_eval = get_max(best_eval, eval);
                alpha = get_max(alpha, eval);
                if (beta <= alpha)
                {
                    memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(uint_fast8_t));
                    moves = moves->next;
                    break;
                }

                if (best_eval == eval) // on a trouvé un nouveau meilleur move
                {
                    meilleur_move = moves->valeur;
                    meilleure_piece = liste_pieces->valeur;
                    best_eval = eval;
                }

                // reset le plateau puis incrémente la liste des moves
                memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(uint_fast8_t));
                moves = moves->next;
            }
            // liberation de moves avant la prochaine itération:
            liberation(moves);
            liste_pieces = liste_pieces->next;
        }
        // liberation de liste_pieces:
        liberation(liste_pieces);

        free(plateau_backup);

        return_minimax->score = best_eval;
        return_minimax->piece = meilleure_piece;
        return_minimax->move = meilleur_move;
        return best_eval; // on retourne l'évaluation maximale
    }

    else // on cherche a minimizer le score
    {
        best_eval = INT_MAX; // on trouvera forcement plus petit
        liste *liste_pieces = (liste *)malloc(sizeof(liste));
        liste_pieces = NULL;
        liste_pieces = liste_moves(couleur, liste_pieces, plateau);

        while (liste_pieces != NULL)
        {
            liste *moves = (liste *)malloc(sizeof(liste));
            moves = NULL;
            moves = get_legal_all(liste_pieces->valeur, moves, plateau);

            while (moves != NULL)
            {
                effectuer_move(liste_pieces->valeur, moves->valeur, plateau); // on effectue le move

                posKey = generate_posKey(plateau, couleur);
                search = search_table(hashtable, posKey);
                if (search != INT_MIN) // si le move a déjà été évalué
                {
                    eval = search; // on récupère direct son éval
                    //fprintf(fp, "\nEval Recuperee");
                }
                else
                {
                    eval = minimax_ht(couleur_ennemie, 0, plateau, profondeur - 1, alpha, beta, fp); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra minimizer)
                    add_entry(hashtable, posKey, eval);  //ancienne méthode, trop lente
                    //fprintf(fp, " %llu | %d\n", posKey, eval); //nouvelle méthode, on remplit un fichier avant l'éxécution puis on l'utilise en lecture
                }
                best_eval = get_min(best_eval, eval);

                beta = get_min(beta, eval);
                if (beta <= alpha)
                {
                    memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(uint_fast8_t));
                    moves = moves->next;
                    break;
                }

                if (eval == best_eval) // on a trouvé un nouveau score minimal
                {
                    meilleur_move = moves->valeur;
                    meilleure_piece = liste_pieces->valeur;

                    best_eval = eval;
                }

                memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(uint_fast8_t));
                moves = moves->next;
            }

            liberation(moves);
            liste_pieces = liste_pieces->next;
        }
        liberation(liste_pieces);
        free(plateau_backup);

        return_minimax->score = best_eval;
        return_minimax->piece = meilleure_piece;
        return_minimax->move = meilleur_move;

        return best_eval;
    }
}