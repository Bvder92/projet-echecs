#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){

    int ligne, colonne, position;
    initialiser_jeu();
    
    affichage_echequier(echequier, MAX);
    printf("\nSelectionner une piece:\n\tLigne: ");
    scanf("%d", &ligne);
    printf("\n\tColonne: ");
    scanf("%d", &colonne);

    position = get_pos(ligne, colonne);

    printf("%d", echequier[position]);

    return 0;
}