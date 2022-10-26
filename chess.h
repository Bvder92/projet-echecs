#ifndef __CHESS_H__
#define __CHESS_H__

#define TAILLE 8
#define MAX 64

#define VIDE 0
#define PION 1
#define CAVALIER  2
#define FOU 3
#define TOUR 4
#define REINE 5
#define ROI 6
#define NOIR 128

char echequier[MAX];

void init_echequier();

void placer_pion(int ligne, int colonne);

void affichage_tableau(char * tab, int taille);
   
void affichage_echequier(char * tab, int taille);

void initialiser_jeu();
    
#endif