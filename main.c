#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int echequier[64];

int main(){

    int ligne, colonne, position, rep = 0;
    int move;
    initialiser_jeu();
    affichage_echequier();
    printf("\nTour des Blancs:\n");

    
    printf("\nSelectionner une piece:\n\tColonne: ");
    scanf("%d", &colonne);
    printf("\n\tLigne: ");
    scanf("%d", &ligne);
    position = get_pos(ligne, colonne);
    

    return 0;
}


/* TODO:

-vérifier que tout marche (déplacements pion blanc, noir, cavalier, fou, tour, reine, roi)
-capture:
    -les fontions cavalier et roi retournent -1 pour une case occupée
    -les fontions pions, fou, tour, reine proposent le move de la case occupée (mais pas les suivantes, respect d'obstacles(normalement))
    
    =>Mettre en place un système de capture:
        -idée: 
        si une case est occupée par la couleur alliée: -1
        si elle est occupée par la couleur ennemie: +64
            ->on a juste a afficher la valeur -64 à l'utilisateur et on peut flag quels moves sont capture ou non
        si l'utilisateur séléctionne un move > 64 on appelle une fonction capture qui fait l'échange
*/