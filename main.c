#include "chess.h"
#include <stdio.h>
#include <stdlib.h>



int main(){
    
    /*int ligne, colonne;
    printf("Entrer ligne: ");
    scanf("%d", &ligne);
    printf("\nEntrer colonne: ");
    scanf("%d", &colonne);
    placer_pion(ligne, colonne);*/

    initialiser_jeu();
    affichage_echequier(echequier, MAX);

    return 0;
}