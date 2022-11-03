#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int echequier[64];

int main(){

    int ligne, colonne, position, rep = 0;
    int move;
    initialiser_debug();
    
    affichage_echequier(echequier, MAX);
    while (rep != 2){

        printf("\nSelectionner une piece:\n\tColonne: ");
        scanf("%d", &colonne);
        printf("\n\tLigne: ");
        scanf("%d", &ligne);

        position = get_pos(ligne, colonne);
        move = bouger(position);
        if ((move < 64) && (move >= 0)){
            echequier[move] = echequier[position];
            echequier[position] = 0;
        }
        affichage_echequier(echequier, MAX);
        printf("\n*Continuer? (1: oui, 2: non): ");
        scanf("%d", &rep);
    }

    return 0;
}
