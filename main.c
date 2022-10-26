#include "chess.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    

    initialiser_jeu();
    affichage_echequier(echequier, MAX);
    bouger_pion(55);
    affichage_echequier(echequier, MAX);
    bouger_pion(39);
    affichage_echequier(echequier, MAX);
    
    return 0;
}