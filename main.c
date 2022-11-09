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