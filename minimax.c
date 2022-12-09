#include "chess.h"
#include <stdio.h>
#include <stdlib.h>

// retourne la valeur de la piece dans position
int get_valeur(int position, int * tab)
{
    int valeur;
    switch (echequier[position])
    {
    case VIDE:
        valeur = 0;
        break;
    case PION:
    case PION + SPECIAL:
    case PION + NOIR:
    case PION + NOIR + SPECIAL:
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
        i = 0; //i commence à 0 car les noirs commencent en haut
        while (i < MAX && nb_pieces < 16) //il n'y a jamais plus de 16 pieces d'une couleur donc on peut d'arreter quand on en compte 16
        {
            if (get_color(i, tab) == 1){
                valeur = valeur + get_valeur(i, tab);
                nb_pieces++;
            }
            i++;
        }
    }
    else if (couleur == 0){
        i = MAX;
        while (i > 0 && nb_pieces < 16){
            if (get_color(i, tab) == 0){
                valeur = valeur + get_valeur(i, tab);
                nb_pieces++;
            }
            i--;
        }
    }
    else{
        fprintf(stderr,"\nERREUR FONCTION GET_VALEUR_TOTAL: COULEUR INVALIDE\n");
        return -1;
    }
    return valeur;
}

//retourne le score actuel de la couleur passée en argument: get_valeur_total(couleur)-get_valeur_total(couleur ennemie)
int get_score(int couleur, int * tab)
{
    if (couleur == 1){
        return get_valeur_total(couleur, tab) - get_valeur_total(0, tab);
    }
    else if (couleur == 0){
        return get_valeur_total(couleur, tab) - get_valeur_total(1, tab);
    }
    else{
        fprintf(stderr, "\nERREUR FONCTION GET_SCORE: COULEUR INVALIDE\n");
        return -1;
    }
}

//fais une copie de l'echequier, effectue le move, fais la difference du score de l'echequier copié et du vrai et retourne le score du move