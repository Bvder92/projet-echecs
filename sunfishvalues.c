#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>
#include <string.h>

//utilisé pour inverser les tableaux précédents pour les utiliser sur les pieces noires
const int mirroir[TAILLE_ECHEQUIER] = {
    56, 57, 58, 59, 60, 61, 62, 63,
    48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47,
    32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31,
    16, 17, 18, 19, 20, 21, 22, 23,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7};


const int pion_alt[TAILLE_ECHEQUIER] = {
            0,   0,   0,   0,   0,   0,   0,   0,
            78,  83,  86,  73, 102,  82,  85,  90,
             7,  29,  21,  44,  40,  31,  44,   7,
           -17,  16,  -2,  15,  14,   0,  15, -13,
           -26,   3,  10,   9,   6,   1,   0, -23,
           -22,   9,   5, -11, -10,  -2,   3, -19,
           -31,   8,  -7, -37, -36, -14,   3, -31,
             0,   0,   0,   0,   0,   0,   0,   0};

const int cavalier_alt[TAILLE_ECHEQUIER] = {
             -66, -53, -75, -75, -10, -55, -58, -70,
            -3,  -6, 100, -36,   4,  62,  -4, -14,
            10,  67,   1,  74,  73,  27,  62,  -2,
            24,  24,  45,  37,  33,  41,  25,  17,
            -1,   5,  31,  21,  22,  35,   2,   0,
           -18,  10,  13,  22,  18,  15,  11, -14,
           -23, -15,   2,   0,   2,   0, -23, -20,
           -74, -23, -26, -24, -19, -35, -22, -69};

const int fou_alt [TAILLE_ECHEQUIER] = {
            -59, -78, -82, -76, -23,-107, -37, -50,
           -11,  20,  35, -42, -39,  31,   2, -22,
            -9,  39, -32,  41,  52, -10,  28, -14,
            25,  17,  20,  34,  26,  25,  15,  10,
            13,  10,  17,  23,  17,  16,   0,   7,
            14,  25,  24,  15,   8,  25,  20,  15,
            19,  20,  11,   6,   7,   6,  20,  16,
            -7,   2, -15, -12, -14, -15, -10, -10};

const int tour_alt [TAILLE_ECHEQUIER] =  {
            35,  29,  33,   4,  37,  33,  56,  50,
            55,  29,  56,  67,  55,  62,  34,  60,
            19,  35,  28,  33,  45,  27,  25,  15,
             0,   5,  16,  13,  18,  -4,  -9,  -6,
           -28, -35, -16, -21, -13, -29, -46, -30,
           -42, -28, -42, -25, -25, -35, -26, -46,
           -53, -38, -31, -26, -29, -43, -44, -53,
           -30, -24, -18,   5,  -2, -18, -31, -32};

const int reine_alt [TAILLE_ECHEQUIER] =  {
            6,   1,  -8,-104,  69,  24,  88,  26,
            14,  32,  60, -10,  20,  76,  57,  24,
            -2,  43,  32,  60,  72,  63,  43,   2,
             1, -16,  22,  17,  25,  20, -13,  -6,
           -14, -15,  -2,  -5,  -1, -10, -20, -22,
           -30,  -6, -13, -11, -16, -11, -16, -27,
           -36, -18,   0, -19, -15, -15, -21, -38,
           -39, -30, -31, -13, -31, -36, -34, -42};

const int roi_alt [TAILLE_ECHEQUIER] = {
            4,  54,  47, -99, -99,  60,  83, -62,
           -32,  10,  55,  56,  56,  55,  10,   3,
           -62,  12, -57,  44, -67,  28,  37, -31,
           -55,  50,  11,  -4, -19,  13,   0, -49,
           -55, -43, -52, -28, -51, -47,  -8, -50,
           -47, -42, -43, -79, -64, -32, -29, -32,
            -4,   3, -14, -50, -57, -18,  13,   4,
            17,  30,  -3, -14,   6,  -1,  40,  18};


int get_valeur_materielle_alt(char position, unsigned char *plateau)
{
    int valeur;
    switch (plateau[position])
    {
    case VIDE:
        valeur = 0;
        break;
    case PION:
    case PION + PIECE_SPECIAL:
    case PION + PIECE_NOIRE:
    case PION + PIECE_NOIRE + PIECE_SPECIAL:
        valeur = PION_V;
        break;
    case CAVALIER:
    case CAVALIER + PIECE_NOIRE:
        valeur = CAVALIER_V;
        break;
    case FOU:
    case FOU + PIECE_NOIRE:
        valeur = FOU_V;
        break;
    case TOUR:
    case TOUR + PIECE_NOIRE:
        valeur = TOUR_V;
        break;
    case REINE:
    case REINE + PIECE_NOIRE:
        valeur = REINE_V;
        break;
    case ROI:
    case ROI + PIECE_SPECIAL:
    case ROI + PIECE_NOIRE:
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
        valeur = ROI_V;
        break;
    default:
        fprintf(stderr, "\nFONCTION GET_VALEUR: PIECE INCONNUE\n");
        valeur = -1;
    }
    return valeur;
}

int get_bonus_placements_alt(unsigned char piece, char position)
{
    switch (piece)
    {
    case VIDE:
        break;
    case PION:
    case PION + PIECE_SPECIAL:
        return pion_alt[position];
        break;
    case CAVALIER:
        return cavalier_alt[position];
        break;
    case FOU:
        return fou_alt[position];
        break;
    case TOUR:
        return tour_alt[position];
        break;
    case REINE:
        return reine_alt[position];
        break;
    case ROI:
    case ROI + PIECE_SPECIAL:
        if (fen->endgame == 1)
        {
            return roi_alt[position];
            break;
        }
        else
        {
            return roi_alt[position];
            break;
        }

    // POUR LES PIECES NOIRES ON UTILISE TABLE[TABLE_MIRROIR[POSITION]]
    case PION + PIECE_NOIRE:
    case PION + PIECE_NOIRE + PIECE_SPECIAL:
        return pion_alt[mirroir[position]];
        break;
    case CAVALIER + PIECE_NOIRE:
        return cavalier_alt[mirroir[position]];
        break;
    case FOU + PIECE_NOIRE:
        return fou_alt[mirroir[position]];
        break;
    case TOUR + PIECE_NOIRE:
        return tour_alt[mirroir[position]];
        break;
    case REINE + PIECE_NOIRE:
        return reine_alt[mirroir[position]];
        break;
    case ROI + PIECE_NOIRE:
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
        if (fen->endgame == 1)
        {
            return roi_alt[mirroir[position]];
            break;
        }
        else
        {
            return roi_alt[mirroir[position]];
            break;
        }
    default:
        break;
    };
}

// retourne la somme des valeurs des bonus de placement pour chaque piece d'une couleur donnée
int get_bonus_placements_total_alt(char couleur, unsigned char *plateau)
{
    char nb_pieces = compter_pieces(couleur, plateau); // nombre de pieces de "couleur" présentes sur l'echequier
    char i, j = 0;
    int materiel_total, bonus_total;

    if (couleur == NOIR) // les pieces noires commencent la partie entre echequier[0] et 16, i commence donc à 0
    {
        i = 0;
        while (j < nb_pieces)
        {
            if (get_color(plateau[i]) == couleur)
            {
                bonus_total += get_bonus_placements_alt(plateau[i], i);
                ++j;
            }
            ++i;
        }
    }
    else // les pieces blanches commencent entre echequier[63] et 48, i commence donc à 63 et décrémente
    {
        i = 63;
        while (j < nb_pieces)
        {
            if (get_color(plateau[i]) == couleur)
            {
                bonus_total += get_bonus_placements_alt(plateau[i], i);
                ++j;
            }
            i--;
        }
    }
    return bonus_total;
}
// retourne la somme de la valeur des pieces d'une couleur passée en parametre
int get_valeur_materielle_totale_alt(char couleur, unsigned char *plateau)
{
    char i, j = 0;
    char nb_pieces = compter_pieces(couleur, plateau);
    int materiel_total = 0;

    if (couleur == NOIR)
    {
        i = 0;                // i commence à 0 car les noirs commencent en haut
        while (j < nb_pieces) // il n'y a jamais plus de 16 pieces d'une couleur
        {
            if (get_color(plateau[i]) == couleur)
            {
                materiel_total += get_valeur_materielle_alt(i, plateau);
                ++j;
            }
            ++i;
        }
    }
    else
    {
        i = TAILLE_ECHEQUIER - 1;
        while (j < nb_pieces)
        {
            if (get_color(plateau[i]) == couleur)
            {
                materiel_total += get_valeur_materielle_alt(i, plateau);
                ++j;
            }
            i--;
        }
    }
    return materiel_total;
}

// retourne la somme des valeurs des bonus de placement pour chaque piece d'une couleur donnée
int get_score_couleur_alt(char couleur, unsigned char *plateau)
{
    char nb_pieces = compter_pieces(couleur, plateau);
    int i, j = 0;
    int bonus_total = 0, materiel_total = 0;

    if (couleur == NOIR)
    {
        i = 0;
        while (j < nb_pieces)
        {
            if (get_color(plateau[i]) == couleur)
            {
                materiel_total += get_valeur_materielle_alt(i, plateau);
                bonus_total += get_bonus_placements_alt(plateau[i], i);
                ++j;
            }
            ++i;
        }
    }
    else
    {
        i = 63;
        while (j < nb_pieces)
        {
            if (get_color(plateau[i]) == couleur)
            {
                materiel_total += get_valeur_materielle_alt(i, plateau);
                bonus_total += get_bonus_placements_alt(plateau[i], i);
                ++j;
            }
            i--;
        }
    }
    return bonus_total + materiel_total;
}

// retourne le score total de l'echequier passé en argument, score calculé par rapport aux noirs
int get_score_alt(unsigned char *plateau)
{
    return get_score_couleur_alt(NOIR, plateau) - get_score_couleur_alt(BLANC, plateau);
}

int minimax_alt(char couleur, char maximizer, unsigned char *plateau, char profondeur, int alpha, int beta)
{
    char couleur_ennemie = get_couleur_ennemie(couleur);
    if (profondeur == 0 || echec_et_mat(couleur, plateau) != -1)
    {
        return get_score_alt(plateau);
    }

    char meilleur_move;
    unsigned char meilleure_piece;

    char minimizer = get_minimizer(maximizer);

    int best_eval;
    int eval;

    char i = 0;
    unsigned char *plateau_backup = (unsigned char *)malloc(sizeof(unsigned char) * TAILLE_ECHEQUIER);
    memcpy(plateau_backup, plateau, TAILLE_ECHEQUIER * sizeof(unsigned char)); // utilisé pour reset l'echequier après qu'on ait testé un move

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

                eval = minimax_alt(couleur_ennemie, 0, plateau, profondeur - 1, alpha, beta); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra minimizer)
                best_eval = get_max(best_eval, eval);
                alpha = get_max(alpha, eval);
                if (beta <= alpha)
                {
                    memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(unsigned char));
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
                memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(unsigned char));
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

                eval = minimax_alt(couleur_ennemie, 1, plateau, profondeur - 1, alpha, beta); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra maximizer)

                best_eval = get_min(best_eval, eval);

                beta = get_min(beta, eval);
                if (beta <= alpha)
                {
                    memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(unsigned char));
                    moves = moves->next;
                    break;
                }

                if (eval == best_eval) // on a trouvé un nouveau score minimal
                {
                    meilleur_move = moves->valeur;
                    meilleure_piece = liste_pieces->valeur;

                    best_eval = eval;
                }

                memcpy(plateau, plateau_backup, TAILLE_ECHEQUIER * sizeof(unsigned char));
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