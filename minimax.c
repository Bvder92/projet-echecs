#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

// retourne la valeur de la piece dans position
int get_valeur(char position, unsigned char *plateau)
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
int get_valeur_total(char couleur, unsigned char *plateau)
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
                valeur = valeur + get_valeur(i, plateau);
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
                valeur = valeur + get_valeur(i, plateau);
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

// retourne le score total de l'echequier passé en argument, score calculé par rapport aux noirs
int get_score(unsigned char *plateau)
{
    // score noir + somme des valeurs des pieces blanches capturées
    int score_noir = get_valeur_total(NOIR, plateau);
    /*if (plateau[6] == ROI+PIECE_NOIRE+PIECE_SPECIAL || plateau[2] == ROI+PIECE_NOIRE+PIECE_SPECIAL)
    {
        score_noir = score_noir + 50;
    }
    if (plateau[27] == CAVALIER+PIECE_NOIRE || plateau[28] == CAVALIER+PIECE_NOIRE || plateau[35] == CAVALIER+PIECE_NOIRE || plateau[36] == CAVALIER+PIECE_NOIRE)
    {
        score_noir = score_noir + 1000;
    }*/
    return score_noir + (2400 - get_valeur_total(BLANC, plateau));
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
    echequier[35] = ROI+PIECE_SPECIAL;
    affichage_echequier();
}

// couleur = couleur du joueur qui doit jouer, maximizer = es-ce qu'il veut maximizer ou minimizer son score
int minimax(char couleur, char maximizer, unsigned char *plateau, char profondeur)
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
    unsigned char *plateau_backup = (unsigned char *)malloc(sizeof(unsigned char)*TAILLE_ECHEQUIER);
    plateau_backup = copie_echequier(plateau, plateau_backup); //utilisé pour reset l'echequier après qu'on ait testé un move

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

                // on evalue l'echequier de la perspective de l'ennemi (il va vouloir minimizer donc maximizer = 0)
                eval = minimax(couleur_ennemie, 0, plateau, profondeur - 1);
                if (best_eval < eval)//on a trouvé un nouveau meilleur move
                {
                    meilleur_move = moves->valeur;
                    meilleure_piece = liste_pieces->valeur;
                    best_eval = eval;
                }

                //reset le plateau puis incrémente la liste des moves
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
        return_minimax.move = meilleur_move;
        return_minimax.piece = meilleure_piece;
        return_minimax.score = best_eval;
        return best_eval; // on retourne l'évaluation maximale
    }

    else if (maximizer == 0)// on cherche a minimizer le score
    {
        best_eval = INT_MAX; //on trouvera forcement plus petit
        liste * liste_pieces = (liste*)malloc(sizeof(liste));
        liste_pieces = NULL;
        liste_pieces = liste_moves(couleur, liste_pieces, plateau);

        while (liste_pieces != NULL)
        {
            liste * moves = (liste*)malloc(sizeof(liste));
            moves = NULL;
            moves = get_legal_all(liste_pieces->valeur, moves, plateau);

            while(moves != NULL)
            {
                effectuer_move(liste_pieces->valeur, moves->valeur, plateau); //on effectue le move
                
                eval = minimax(couleur_ennemie, 1, plateau, profondeur-1); //on appelle minimax du point de vue de l'ennemi (il voudra maximiser)
                //printf("\neval = %d", eval);
                if (eval<best_eval) //on a trouvé un nouveau score minimal
                {
                    meilleur_move = moves->valeur;
                    meilleure_piece = liste_pieces->valeur;

                    best_eval = eval;
                }

                plateau = copie_echequier(plateau_backup, plateau); //reset echequier
                
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