#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_echequier(){
    for (int i = 0; i<MAX; i++){
        echequier[i] = '0';
    }
}

void placer_pion(int ligne, int colonne){
    int i = (ligne -1)*8;
    i = i + colonne-1;
    echequier[i] = '1';
}

int get_pos(int ligne, int colonne){
    int i = (ligne -1)*8;
    return i + (colonne-1);
}

void affichage_tableau(char * tab, int taille){
    for (int i = 0; i<taille; i++){
        printf("%c | ", tab[i]);
    }
}

void affichage_echequier(char * tab, int taille){
    int c = 0;
    for (int i = 0; i<taille; i++){
        printf("%d | ", tab[i]);
        c++;
        if (c == 8){
            printf("\n");
            c = 0;
        }
    }
}

void initialiser_jeu(){
    int i;
    echequier[0] = TOUR+NOIR;
    echequier[1] = CAVALIER+NOIR;
    echequier[2] = FOU+NOIR;
    echequier[3] = REINE+NOIR;
    echequier[4] = ROI+NOIR;
    echequier[5] = FOU+NOIR;
    echequier[6] = CAVALIER+NOIR;
    echequier[7] = TOUR+NOIR;
    for (i = 8; i<16; i++){
        echequier[i] = PION+NOIR;
    }
    for ( i = 16; i<48; i++){
        echequier[i] = VIDE;
    }
    for (i = 48; i<56; i++){
        echequier[i] = PION;
    }
    echequier[56] = TOUR;
    echequier[57] = CAVALIER;
    echequier[58] = FOU;
    echequier[59] = REINE;
    echequier[60] = ROI;
    echequier[61] = FOU;
    echequier[62] = CAVALIER;
    echequier[63] = TOUR;
}

int get_ligne(int position){
    return (position/8);
}

int get_colonne(int position){
    int col;
    col = position - (get_ligne(position)*8);
    return col;
}

void print_name(int piece){
    switch (piece){
        case 0: 
            printf("vide");
            break;
        case 1:
            printf("pion");
            break;
        case 2:
            printf("cavalier");
            break;
        case 3:
            printf("fou");
            break;
        case 4:
            printf("tour");
            break;
        case 5:
            printf("reine");
            break;
        case 6:
            printf("roi");
            break;
        default:
            printf("ERREUR");
    }
}

/*
-> Pour l'instant la fonction affiche les coordonnées des mouvements légaux sous forme d'indice du tableau echequier
-> Le but serait qu'elle l'affiche sous forme <lettre><chiffre> genre 'A4'
-> Faut écrire une fonction qui fait l'inverse de "placer_pion" : on lui donne un indice et elle nous renvoie ligne et colonne
*/
void bouger_pion(int position){ //si on entre dans cette fonction on sait qu'il y a un pion dans la position passée en argument, ca sera vérifié dans le main
    int reponse = 0;
    if (128 % echequier[position] == 128) { // le pion est noir
            
        if(position >= 8 && position <= 15){ // le pion n'a jamais bougé donc il peut avancer d'une ou deux cases
            printf("~Moves légaux: 1) (%d,%d) ; 2) (%d,%d)", get_colonne(position+8), get_ligne(position+8),  get_colonne(position+16), get_ligne(position+16)); 
            while (reponse != 1 && reponse != 2){ //on demande a l'utilisateur tant que la rep est incorrecte
                printf("\n1 ou 2: ");
                scanf("%d", &reponse);
            }
            if (reponse == 1) {
                echequier[position + 8] = echequier[position];
                echequier[position] = VIDE;
            }
            else {
                echequier[position + 16] = echequier[position];
                echequier[position] = VIDE;
            }
        }
        else { //le pion a déja été déplacé, on le bouge direct pcq flemme
            echequier[position + 8] = echequier[position];
            echequier[position] = VIDE;
        }
    }

    else{ // le pion est blanc: on fait la meme chose qu'avant mais avec d'autres indices
        if(position >= 48 && position <= 55){ // le pion n'a jamais bougé donc il peut avancer d'une ou deux cases
            printf("~Moves légaux: 1)%d ; 2)%d", position-8, position-16); 
            while (reponse != 1 && reponse != 2){ //on demande a l'utilisateur tant que la rep est incorrecte
                printf("\n1 ou 2: ");
                scanf("%d", &reponse);
            }
            if (reponse == 1) {
                echequier[position - 8] = echequier[position];
                echequier[position] = VIDE;
            }
            else {
                echequier[position - 16] = echequier[position];
                echequier[position] = VIDE;
            }
        }
        else{ // le pion a déja été déplacé, on le bouge direct pcq flemme
            echequier[position - 8] = echequier[position];
            echequier[position] = VIDE;
        }
    }
}