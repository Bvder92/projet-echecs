#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//hello world

// retourne la valeur de la piece dans position
int get_valeur(int position, int *tab)
{
    int valeur;
    switch (tab[position])
    {
    case VIDE:
        valeur = 0;
        break;
    case PION:
    case PION + NOIR:
        valeur = VALEUR_PION;
        break;
    case CAVALIER:
    case CAVALIER + NOIR:
    case FOU:
    case FOU + NOIR:
        valeur = VALEUR_CAVALIER;
        break;
    case TOUR:
    case TOUR + NOIR:
        valeur = VALEUR_TOUR;
        break;
    case REINE:
    case REINE + NOIR:
        valeur = VALEUR_REINE;
        break;
    case ROI:
    case ROI + SPECIAL:
    case ROI + NOIR:
    case ROI + NOIR + SPECIAL:
        valeur = VALEUR_ROI;
        break;
    default:
        fprintf(stderr, "\nFONCTION GET_VALEUR: PIECE INCONNUE\n");
        valeur = -1;
    }
    return valeur;
}

// retourne la somme de la valeur des pieces d'une couleur passée en parametre
int get_valeur_total(int couleur, int *tab)
{
    int i, valeur = 0, nb_pieces = 0;

    if (couleur == 1)
    {
        i = 0;                            // i commence à 0 car les noirs commencent en haut
        while (i < MAX && nb_pieces < 16) // il n'y a jamais plus de 16 pieces d'une couleur donc on peut d'arreter quand on en compte 16
        {
            if (get_color(i, tab) == 1)
            {
                valeur = valeur + get_valeur(i, tab);
                nb_pieces++;
            }
            i++;
        }
    }
    else if (couleur == 0)
    {
        i = MAX;
        while (i > 0 && nb_pieces < 16)
        {
            if (get_color(i, tab) == 0)
            {
                valeur = valeur + get_valeur(i, tab);
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

// retourne le score actuel de la couleur passée en argument: get_valeur_total(couleur)-get_valeur_total(couleur ennemie)
int get_score(int couleur, int *tab)
{
    /*if (couleur == 1)
    {
        return get_valeur_total(couleur, tab) - get_valeur_total(0, tab); // score noir - score blanc
    }
    else if (couleur == 0)
    {
        return get_valeur_total(couleur, tab) - get_valeur_total(1, tab); // score blanc - score noir
    }
    else
    {
        fprintf(stderr, "\nERREUR FONCTION GET_SCORE: COULEUR INVALIDE\n");
        return -1;
    }*/
    return get_valeur_total(0, tab) + (1290 - get_valeur_total(1, tab));
}

/* retourne "la valeur" d'un move: la différence entre le score de l'echequier après ce move et le score de l'echequier avant
int get_score_move(int position, int move) // position = position de la piece dont on étudie les moves, move = un move possible de la piece étudiée
{
    int *echequier_tmp;
    int score, score_tmp, couleur;

    couleur = get_color(position, echequier);

    echequier_tmp = (int *)malloc(sizeof(int) * MAX);
    echequier_tmp = copie_echequier(echequier, echequier_tmp);

    // on effectue le move:
    echequier_tmp[move] = echequier_tmp[position];
    echequier_tmp[position] = VIDE;

    // calcul du score:
    score = get_score(couleur, echequier);
    score_tmp = get_score(couleur, echequier_tmp);
    score_tmp = score_tmp - score;

    // liberation et retour:
    free(echequier_tmp);
    echequier_tmp = NULL;
    return score_tmp;
}

// retourne un tableau contenant "la valeur" de chaque move d'une pièce:
int *get_score_all_moves(int position, FEN fen)
{
    // declarations et allocation de moves:
    int *scores; // tableau contenant le score de chaque move (scores[0] = score de moves[0])
    int *moves, taille_moves, taille_scores, i = 0;

    taille_moves = get_taille_moves(position, echequier);
    moves = (int *)malloc(sizeof(int) * taille_moves);
    moves = get_moves_total(moves, taille_moves, position, fen.castleb, fen.castlew, echequier);
    if (moves == NULL)
    {
        fprintf(stderr, "\nERREUR FONCTION GET_SCORE_ALL_MOVES: TABLEAU MOVES NULL\n");
        return NULL;
    }

    /* calcul taille scores puis allocation:
    while (moves[i] != -1 && i < taille_moves)
    {
        taille_scores++;
        i++;
    } //
    taille_scores = taille_moves;
    scores = (int *)malloc(sizeof(int) * taille_scores);
    if (moves == NULL)
    {
        fprintf(stderr, "\nERREUR FONCTION GET_SCORE_ALL_MOVES: TABLEAU SCORES NULL\n");
        return NULL;
    }

    // remplissage de scores:
    i = 0;
    while (i < taille_scores && moves[i] != -1)
    {
        scores[i] = get_score_move(position, moves[i]);
        i++;
    }

    while (i < taille_moves)
    {
        scores[i] = moves[i];
        i++;
    }

    // liberation et retour:
    free(moves);
    moves = NULL;
    return scores;
}



// retourne le meilleur move possible pour le joueur de couleur_maximizer.
int minimax(int position, int profondeur, int couleur_maximizer)
{
    int *liste_pieces;
    int taille_liste_pieces, i;
    int couleur_minimizer;
    int eval;
    int max_eval;
    int min_eval;

    if (couleur_maximizer = 1)
    {
        couleur_minimizer = 0;
    }
    else
    {
        couleur_minimizer = 1;
    }

    if (profondeur == 0)
    {
        return get_score(couleur_maximizer, echequier);
    }

    // les noirs = maximizers, blancs minimizers => les noirs veulent un score positif et les blancs un score négatif
    if (couleur_maximizer == 1)
    {
        taille_liste_pieces = compter_noir();
        liste_pieces = (int *)malloc(sizeof(int) * taille_liste_pieces);
        liste_pieces = liste_moves(couleur_maximizer, liste_pieces, taille_liste_pieces);
        if (liste_pieces == NULL)
        {
            fprintf(stderr, "\nERREUR FONTION MINIMAX: LISTE_PIECES NULL\n");
            return INT_MIN;
        }

        max_eval = INT_MIN;
        for (i = 0; i < taille_liste_pieces; i++)
        {
            eval = minimax(i, profondeur - 1, couleur_minimizer);
            max_eval = get_max(max_eval, eval);
        }
        return max_eval;
    }
    else
    {
        taille_liste_pieces = compter_blanc();
        liste_pieces = (int *)malloc(sizeof(int) * taille_liste_pieces);
        liste_pieces = liste_moves(couleur_maximizer, liste_pieces, taille_liste_pieces);

        min_eval = INT_MAX;
        for (i = 0; i < taille_liste_pieces; i++)
        {
            eval = minimax(i, profondeur - 1, couleur_maximizer);
            min_eval = get_min(min_eval, eval);
        }
        return min_eval;
    }
}

int minimaxx(int position, int maximizer, int profondeur)
{
    int couleur = get_color(position, echequier);
    if (profondeur == 0)
    {
        return get_score(couleur, echequier);
    }

    int *liste_pieces;
    int taille_liste_pieces, minimizer, eval, max_eval, min_eval, i;

    if (maximizer == 1) minimizer = 0;
    else minimizer = 1;

    if (couleur == 1)
    {
        taille_liste_pieces = compter_noir();
    }
    else if (couleur == 0)
    {
        taille_liste_pieces = compter_blanc();
    }

    liste_pieces = (int *)malloc(sizeof(int)*taille_liste_pieces);
    if (liste_pieces == NULL)
    {
        fprintf(stderr, "erreur minimax");
        return INT_MIN;
    }
    liste_pieces = liste_moves(couleur, liste_pieces, taille_liste_pieces, );
    
    if (maximizer == 1) // on cherche a maximizer le score (calculé par rapport aux noirs):
    {
        max_eval = INT_MIN;
        for (i = 0; i < taille_liste_pieces; i++) // pour chaque piece de notre couleur pouvant bouger:
        {
            eval = minimaxx(i, minimizer, profondeur - 1); // on cherche a minimizer notre score
            max_eval = get_max(max_eval, eval);            // on conserve le score qui nous arrange le plus parmi les scores minimizés
        }
        free(liste_pieces);
        liste_pieces == NULL;
        return max_eval;
    }

    else // on cherche a minimizer le score:
    {
        min_eval = INT_MAX;
        for (i = 0; i < taille_liste_pieces; i++) // pour chaque piece de notre couleur
        {
            eval = minimaxx(i, maximizer, profondeur - 1); // on cherhce a maximizer le score
            min_eval = get_min(min_eval, eval);            // on garde le score maximizé minimum
        }
        free(liste_pieces);
        liste_pieces == NULL;
        return min_eval;
    }
}
*/

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

int get_minimizer(int maximizer){
    if (maximizer ==  1){
        return 0;
    }
    else return 1;
}

int get_couleur_ennemie(int couleur){
    if (couleur == 1){
        return 0;
    }
    else return 1;
}

//maximizer = on maximize oui ou non 
int encore(int maximizer, int couleur, int profondeur, int * plateau, int alpha, int beta){
    
    int * liste_pieces, * moves, taille_moves, taille_liste_pieces, minimizer, eval, couleur_ennemie, i = 0, j = 0;

    minimizer = get_minimizer(maximizer);
    couleur_ennemie = get_couleur_ennemie(couleur);
    taille_liste_pieces = compter(couleur, plateau);

    liste_pieces = (int *)malloc(sizeof(int)*taille_liste_pieces);
    if (liste_pieces == NULL)
    {
        fprintf(stderr, "erreur minimax");
        return INT_MIN;
    }
    liste_pieces = liste_moves(couleur, liste_pieces, taille_liste_pieces, plateau);//on recupere toutes les pieces ayant des moves de la couleur appelante

    if (profondeur == 0 || liste_pieces[0] == -1)
    {
        return get_score(couleur, plateau);
    }

    if (maximizer == 1) // on cherche a maximizer le score
    {
        int max_eval = INT_MAX;
        while (i < taille_liste_pieces && liste_pieces[i] != -1) //pour chaque piece qui peut bouger
        {
            taille_moves = get_taille_moves(liste_pieces[i], plateau);
            moves = (int *)malloc(sizeof(int)*taille_moves);
            if (moves == NULL){
                fprintf(stderr, "\nMOVES NULL\n");
                return INT_MIN;
            }
            moves = get_moves_total(moves, taille_moves, liste_pieces[i], castle(4, plateau), castle(60, plateau), plateau); //on recupere tous les moves de la piece

            while (j < taille_moves && moves[j] != -1) //pour chaque move de la piece
            {
                //on effectue le move dans un echequier temporaire
                int * echequier_tmp = (int *)malloc(sizeof(int)*MAX);
                if (echequier_tmp == NULL){
                    fprintf(stderr, "\nECHEQUIER_TMP NULL\n");
                    return INT_MIN;
                }
                echequier_tmp = copie_echequier(plateau, echequier_tmp);
                echequier_tmp[moves[j]] =  echequier_tmp[liste_pieces[i]]; //moves[j] = position ou la piece doit aller, liste_pieces[i] = position de la piece actuelle
                echequier_tmp[liste_pieces[i]] = VIDE;

                //on se met a la place de l'ennemi après ce move:
                eval = encore(minimizer, couleur_ennemie, profondeur-1, echequier_tmp, alpha, beta); //retourne le plus petit score possible après un move ennemi
                max_eval = get_max(max_eval, eval);
                alpha = get_max(alpha, eval);
                if (beta <= alpha){
                    break;
                }

                free(echequier_tmp);
                echequier_tmp = NULL;
            }
            free(moves);
            moves = NULL;
        }
        free(liste_pieces);
        liste_pieces = NULL;
        return max_eval;
    }
    else{
        int min_eval = INT_MIN;
        while (i < taille_liste_pieces && liste_pieces[i] != -1) //pour chaque piece qui peut bouger
        {
            taille_moves = get_taille_moves(liste_pieces[i], plateau);
            moves = (int *)malloc(sizeof(int)*taille_moves);
            if (moves == NULL){
                fprintf(stderr, "\nMOVES NULL\n");
                return INT_MIN;
            }
            moves = get_moves_total(moves, taille_moves, liste_pieces[i], castle(4, plateau), castle(60, plateau), plateau); //on recupere tous les moves de la piece

            while (j < taille_moves && moves[j] != -1) //pour chaque move de la piece
            {
                //on effectue le move dans un echequier temporaire
                int * echequier_tmp = (int *)malloc(sizeof(int)*MAX);
                if (echequier_tmp == NULL){
                    fprintf(stderr, "\nECHEQUIER_TMP NULL\n");
                    return INT_MIN;
                }
                echequier_tmp = copie_echequier(plateau, echequier_tmp);
                echequier_tmp[moves[j]] =  echequier_tmp[liste_pieces[i]]; //moves[j] = position ou la piece doit aller, liste_pieces[i] = position de la piece actuelle
                echequier_tmp[liste_pieces[i]] = VIDE;

                //on se met a la place de l'ennemi après ce move:
                eval = encore(maximizer, couleur_ennemie, profondeur-1, echequier_tmp, alpha, beta); //retourne le plus grand score possible après un move ennemi
                min_eval = get_min(min_eval, eval);
                beta = get_min(min_eval, eval);
                if (beta <= alpha){
                    break;
                }

                free(echequier_tmp);
                echequier_tmp = NULL;
            }
            free(moves);
            moves = NULL;
        }
        free(liste_pieces);
        liste_pieces = NULL;
        return min_eval;
    }
}
