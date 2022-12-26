#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

/* *******************************************************
TABLES DES CASES OPTIMALES POUR CHAQUE PIECE (BLANCHES):
******************************************************* */

const int table_pion[TAILLE_ECHEQUIER] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5, 5, 10, 25, 25, 10, 5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, -5, -10, 0, 0, -10, -5, 5,
    5, 10, 10, -20, -20, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0};

const int table_cavalier[TAILLE_ECHEQUIER] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50};

const int table_fou[TAILLE_ECHEQUIER] = {
    -20, -10, -10, -10, -10, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 10, 10, 5, 0, -10,
    -10, 5, 5, 10, 10, 5, 5, -10,
    -10, 0, 10, 10, 10, 10, 0, -10,
    -10, 10, 10, 10, 10, 10, 10, -10,
    -10, 5, 0, 0, 0, 0, 5, -10,
    -20, -10, -10, -10, -10, -10, -10, -20};

int table_tour[TAILLE_ECHEQUIER] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    5, 10, 10, 10, 10, 10, 10, 5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    -5, 0, 0, 0, 0, 0, 0, -5,
    0, 0, 0, 5, 5, 0, 0, 0};

const int table_reine[TAILLE_ECHEQUIER] = {
    -20, -10, -10, -5, -5, -10, -10, -20,
    -10, 0, 0, 0, 0, 0, 0, -10,
    -10, 0, 5, 5, 5, 5, 0, -10,
    -5, 0, 5, 5, 5, 5, 0, -5,
    0, 0, 5, 5, 5, 5, 0, -5,
    -10, 5, 5, 5, 5, 5, 0, -10,
    -10, 0, 5, 0, 0, 0, 0, -10,
    -20, -10, -10, -5, -5, -10, -10, -20};

const int table_roi[TAILLE_ECHEQUIER] = {
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -30, -40, -40, -50, -50, -40, -40, -30,
    -20, -30, -30, -40, -40, -30, -30, -20,
    -10, -20, -20, -20, -20, -20, -20, -10,
    20, 20, 0, 0, 0, 0, 20, 20,
    20, 30, 10, 0, 0, 10, 30, 20};

//utilisé pour inverser les tableaux précédents pour les utiliser sur les pieces noires
const int table_mirroir[TAILLE_ECHEQUIER] = {
    56, 57, 58, 59, 60, 61, 62, 63,
    48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47,
    32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31,
    16, 17, 18, 19, 20, 21, 22, 23,
    8, 9, 10, 11, 12, 13, 14, 15,
    0, 1, 2, 3, 4, 5, 6, 7};

// retourne la valeur de la piece dans position
int get_valeur_materielle(char position, unsigned char *plateau)
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
        valeur = VALEUR_PION;
        break;
    case CAVALIER:
    case CAVALIER + PIECE_NOIRE:
    case FOU:
    case FOU + PIECE_NOIRE:
        valeur = VALEUR_CAVALIER;
        break;
    case TOUR:
    case TOUR + PIECE_NOIRE:
        valeur = VALEUR_TOUR;
        break;
    case REINE:
    case REINE + PIECE_NOIRE:
        valeur = VALEUR_REINE;
        break;
    case ROI:
    case ROI + PIECE_SPECIAL:
    case ROI + PIECE_NOIRE:
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
        valeur = VALEUR_ROI;
        break;
    default:
        fprintf(stderr, "\nFONCTION GET_VALEUR: PIECE INCONNUE\n");
        valeur = -1;
    }
    return valeur;
}

// retourne la somme de la valeur des pieces d'une couleur passée en parametre
int get_valeur_materielle_totale(char couleur, unsigned char *plateau)
{
    char i, nb_pieces = 0;
    int valeur = 0;

    if (couleur == NOIR)
    {
        i = 0;                                         // i commence à 0 car les noirs commencent en haut
        while (i < TAILLE_ECHEQUIER && nb_pieces < 16) // il n'y a jamais plus de 16 pieces d'une couleur
        {
            if (get_color(plateau[i]) == couleur)
            {
                valeur = valeur + get_valeur_materielle(i, plateau);
                nb_pieces++;
            }
            i++;
        }
    }
    else if (couleur == BLANC)
    {
        i = TAILLE_ECHEQUIER - 1;
        while (i >= 0 && nb_pieces < 16)
        {
            if (get_color(plateau[i]) == couleur)
            {
                valeur = valeur + get_valeur_materielle(i, plateau);
                nb_pieces++;
            }
            i--;
        }
    }
    else
    {
        fprintf(stderr, "\nERREUR FONCTION GET_VALEUR_TOTAL: COULEUR INVALIDE\n");
        return -1;
    }
    return valeur;
}

int get_bonus_placements(unsigned char piece, char position)
{
    switch (piece)
    {
    case VIDE:
        break;
    case PION:
    case PION + PIECE_SPECIAL:
        return table_pion[position];
        break;
    case CAVALIER:
        return table_cavalier[position];
        break;
    case FOU:
        return table_fou[position];
        break;
    case TOUR:
        return table_tour[position];
        break;
    case REINE:
        return table_reine[position];
        break;
    case ROI:
    case ROI + PIECE_SPECIAL:
        return table_roi[position];
        break;
    
    //POUR LES PIECES NOIRES ON UTILISE TABLE[TABLE_MIRROIR[POSITION]] 
    case PION + PIECE_NOIRE:
    case PION + PIECE_NOIRE + PIECE_SPECIAL:
        return table_pion[table_mirroir[position]]; 
        break;
    case CAVALIER + PIECE_NOIRE:
        return table_cavalier[table_mirroir[position]];
        break;
    case FOU + PIECE_NOIRE:
        return table_fou[table_mirroir[position]];
        break;
    case TOUR + PIECE_NOIRE:
        return table_tour[table_mirroir[position]];
        break;
    case REINE + PIECE_NOIRE:
        return table_reine[table_mirroir[position]];
        break;
    case ROI + PIECE_NOIRE:
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
        return table_roi[table_mirroir[position]];
        break;
    default:
        break;
    };
}

//retourne la somme des valeurs des bonus de placement pour chaque piece d'une couleur donnée
int get_bonus_placements_total(char couleur, unsigned char * plateau)
{
    char nb_pieces = compter_pieces(couleur, plateau); //nombre de pieces de "couleur" présentes sur l'echequier
    int i, bonus_total;

    if (couleur == NOIR) //les pieces noires commencent la partie entre echequier[0] et 16, i commence donc à 0
    {
        i = 0;
        while (i<TAILLE_ECHEQUIER && i<nb_pieces)
        {
            if (get_color(plateau[i]) == couleur)
            {
                bonus_total += get_bonus_placements(plateau[i], i);
            }
            ++i;
        }
    }
    else //les pieces blanches commencent entre echequier[63] et 48, i commence donc à 63 et décrémente
    {
        i = 63;
        while (i>0 && i<nb_pieces)
        {
            if (get_color(plateau[i]) == couleur)
            {
                bonus_total += get_bonus_placements(plateau[i], i);
            }
            --i;
        }
    }
    return bonus_total;
}

// retourne le score total de l'echequier passé en argument, score calculé par rapport aux noirs
int get_score(unsigned char *plateau)
{
    // score noir + somme des valeurs des pieces blanches capturées
    int score_noir = get_valeur_materielle_totale(NOIR, plateau) + get_bonus_placements_total(NOIR, plateau);
    int score_blanc = get_valeur_materielle_totale(BLANC, plateau) + get_bonus_placements_total(BLANC, plateau);
    return score_noir + (2400 - get_valeur_materielle_totale(BLANC, plateau));
}

// retourne le plus grand entre les 2 entiers
int get_max(int a, int b)
{
    if (a > b)
        return a;
    else
        return b;
}

int get_min(int a, int b)
{
    if (a > b)
        return b;
    else
        return a;
}

// retourne 0 si on lui passe 1, et 1 si on lui passe 0
char get_minimizer(char maximizer)
{
    if (maximizer == 1)
    {
        return 0;
    }
    else
        return 1;
}

char get_couleur_ennemie(char couleur)
{
    if (couleur == NOIR)
    {
        return BLANC;
    }
    else
    {
        return NOIR;
    }
}

void test()
{
    printf("%d", table_fou[16]);
    printf("\n%d", table_fou[table_mirroir[16]]);
}

// couleur = couleur du joueur qui doit jouer, maximizer = es-ce qu'il veut maximizer ou minimizer son score
int minimax(char couleur, char maximizer, unsigned char *plateau, char profondeur, int alpha, int beta)
{
    char couleur_ennemie = get_couleur_ennemie(couleur);
    if (profondeur == 0 || echec_et_mat(couleur, plateau) != -1)
    {
        // printf("profondeur = 0");
        return get_score(plateau);
    }
    char meilleur_move;
    unsigned char meilleure_piece;

    char minimizer = get_minimizer(maximizer);

    int best_eval;
    int eval;

    char i = 0;
    unsigned char *plateau_backup = (unsigned char *)malloc(sizeof(unsigned char) * TAILLE_ECHEQUIER);
    plateau_backup = copie_echequier(plateau, plateau_backup); // utilisé pour reset l'echequier après qu'on ait testé un move

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

                eval = minimax_mieux(couleur_ennemie, 0, plateau, profondeur - 1, alpha, beta); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra minimizer)
                best_eval = get_max(best_eval, eval);
                alpha = get_max(alpha, eval);
                if(beta <= alpha){
                    break;
                }
                if (best_eval < eval) // on a trouvé un nouveau meilleur move
                {
                    meilleur_move = moves->valeur;
                    meilleure_piece = liste_pieces->valeur;
                    best_eval = eval;
                }

                // reset le plateau puis incrémente la liste des moves
                plateau = copie_echequier(plateau_backup, plateau);
                moves = moves->next;
            }
            // liberation de moves avant la prochaine itération:
            liberation(moves);
            liste_pieces = liste_pieces->next;
        }
        // liberation de liste_pieces:
        liberation(liste_pieces);

        free(plateau_backup);

        return_minimax.score = best_eval;
        return_minimax.piece = meilleure_piece;
        return_minimax.move = meilleur_move;
        return best_eval; // on retourne l'évaluation maximale
    }

    else if (maximizer == 0) // on cherche a minimizer le score
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

                eval = minimax_mieux(couleur_ennemie, 1, plateau, profondeur - 1, alpha, beta); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra maximizer)
                best_eval = get_min(best_eval, eval);
                beta = get_min(beta, eval);

                if(beta <= alpha){
                    break;
                }
                if (eval < best_eval) // on a trouvé un nouveau score minimal
                {
                    meilleur_move = moves->valeur;
                    meilleure_piece = liste_pieces->valeur;

                    best_eval = eval;
                }

                plateau = copie_echequier(plateau_backup, plateau); // reset echequier

                moves = moves->next;
            }

            liberation(moves);
            liste_pieces = liste_pieces->next;
        }
        liberation(liste_pieces);
        free(plateau_backup);

        return_minimax.score = best_eval;
        return_minimax.piece = meilleure_piece;
        return_minimax.move = meilleur_move;

        return best_eval;
    }
}

int minimax_mieux(char couleur, char maximizer, unsigned char *plateau, char profondeur, int alpha, int beta)
{
    if (profondeur == 0 || echec_et_mat(couleur, plateau) != -1)
    {
        return get_score(plateau);
    }

    /* ***********************************************************
    DECALARATIONS:
    *********************************************************** */
    char couleur_ennemie = get_couleur_ennemie(couleur);
    int best_eval;
    int eval;
    unsigned char *plateau_backup = (unsigned char *)malloc(sizeof(unsigned char) * TAILLE_ECHEQUIER);
    plateau_backup = copie_echequier(plateau, plateau_backup); // utilisé pour reset l'echequier après qu'on ait testé un move

    liste *liste_pieces = (liste *)malloc(sizeof(liste)); // liste des pieces pouvant bouger
    liste_pieces = NULL;
    liste_pieces = liste_moves(couleur, liste_pieces, plateau);

    /* ***********************************************************
    ALGO:
    *********************************************************** */

    if (maximizer == 1) // on veut maximiser le score
    {
        best_eval = INT_MIN;

        while (liste_pieces != NULL) // POUR CHAQUE PIECE POUVANT BOUGER
        {
            liste *moves = (liste *)malloc(sizeof(liste)); // liste des moves de la pieces
            moves = NULL;
            moves = get_legal_all(liste_pieces->valeur, moves, plateau); // on récupère tous les moves de la piece

            while (moves != NULL) // POUR CHAQUE MOVE DE LA PIECE
            {
                effectuer_move(liste_pieces->valeur, moves->valeur, plateau); // on effectue le move

                
                eval = minimax_mieux(couleur_ennemie, 0, plateau, profondeur - 1, alpha, beta); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra minimizer)
                best_eval = get_max(best_eval, eval);
                alpha = get_max(alpha, eval);
                if(beta <= alpha){
                    break;
                }

                if (eval > best_eval) // on vient de trouver un nouveau meilleur move
                {
                    return_minimax.move = moves->valeur;
                    return_minimax.piece = liste_pieces->valeur;
                    return_minimax.score = eval;
                    best_eval = eval;
                }

                plateau = copie_echequier(plateau_backup, plateau); // on remet l'echequier dans son état avant le move
                moves = moves->next;
            }
            liberation(moves); // on libère la liste actuelle de moves possibles avant de passer à la pièce suivante
            liste_pieces = liste_pieces->next;
        }

        liberation(liste_pieces);
        free(plateau_backup);
        return best_eval;
    }

    else
    {
        best_eval = INT_MAX;

        while (liste_pieces != NULL) // POUR CHAQUE PIECE POUVANT BOUGER
        {
            liste *moves = (liste *)malloc(sizeof(liste)); // liste des moves de la pieces
            moves = NULL;
            moves = get_legal_all(liste_pieces->valeur, moves, plateau); // on récupère tous les moves de la piece

            while (moves != NULL) // POUR CHAQUE MOVE DE LA PIECE
            {
                effectuer_move(liste_pieces->valeur, moves->valeur, plateau); // on effectue le move

                
                eval = minimax_mieux(couleur_ennemie, 1, plateau, profondeur - 1, alpha, beta); // on évalue l'échequier engendré par le move du point de vue de l'ennemi (il voudra maximizer)
                best_eval = get_min(best_eval, eval);
                beta = get_min(beta, eval);

                if(beta <= alpha){
                    break;
                }
                
                if (eval < best_eval) // on vient de trouver un nouveau meilleur move
                {
                    return_minimax.move = moves->valeur;
                    return_minimax.piece = liste_pieces->valeur;
                    return_minimax.score = eval;
                    best_eval = eval;
                }

                plateau = copie_echequier(plateau_backup, plateau); // on remet l'echequier dans son état avant le move
                moves = moves->next;
            }
            liberation(moves); // on libère la liste actuelle de moves possibles avant de passer à la pièce suivante
            liste_pieces = liste_pieces->next;
        }

        liberation(liste_pieces);
        free(plateau_backup);
        return best_eval;
    }
}