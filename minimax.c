#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
    char i, valeur = 0, nb_pieces = 0;

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
        i = TAILLE_ECHEQUIER;
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
    return get_valeur_total(NOIR, plateau) + (1290 - get_valeur_total(BLANC, plateau));
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
int get_minimizer(char maximizer)
{
    if (maximizer == 1)
    {
        return 0;
    }
    else
        return 1;
}

int get_couleur_ennemie(char couleur)
{
    if (couleur == NOIR)
    {
        return BLANC;
    }
    else
        return NOIR;
}