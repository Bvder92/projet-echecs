#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    int ligne, colonne, position;
    initialiser_jeu();
    
    affichage_echequier(echequier, MAX);
    printf("\nSelectionner une piece:\n\tLigne: ");
    scanf("%d", &ligne);
    printf("\n\tColonne: ");
    scanf("%d", &colonne);

    //position = get_pos(ligne, colonne);

    for (position = 48; position<64; position++){
        printf("\nNom de la piece: ");
        print_name(echequier[position]);
    }
    return 0;
}