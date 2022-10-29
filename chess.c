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
    int i = (ligne)*8;
    return i + (colonne);
}

void affichage_tableau(char * tab, int taille){
    for (int i = 0; i<taille; i++){
        printf("%c | ", tab[i]);
    }
}

void affichage_echequier(int * tab, int taille){
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

void initialiser_debug(){
    int i;
    for (i = 0; i < 63; i++){
        echequier[i] = VIDE;
    }
    echequier[52] = TOUR;
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
        case VIDE: 
            printf("vide");
            break;
        case PION:
            printf("pion");
            break;
        case CAVALIER:
            printf("cavalier");
            break;
        case FOU:
            printf("fou");
            break;
        case TOUR:
            printf("tour");
            break;
        case REINE:
            printf("reine");
            break;
        case ROI:
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

/* Retourne un tableau contenant les indice des 8 cases des 8 moves possibles pour le cavalier
Si la valeur est -1, le move est impossible car hors limite */
int * get_legal_cavalier(int position, int * moves){
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    moves[0] = position - 17;
    moves[1] = position - 15;
    moves[2] = position - 10;
    moves[3] = position - 6;
    moves[4] = position + 6;
    moves[5] = position + 10;
    moves[6] = position + 15;
    moves[7] = position + 17;

    if (ligne == 0) moves[0] = moves[1] = moves[2] = moves[3] = -1;
    if (ligne == 1) moves[0] = moves[1] = -1;
    if (ligne == 6) moves[6] = moves[7] = -1;
    if (ligne == 7) moves[4] = moves[5] = moves[6] = moves[7] = -1;

    if (colonne == 0) moves[0] = moves[2] = moves[4] = moves[6] = -1;
    if (colonne == 1) moves[2] = moves[4] = -1;
    if (colonne == 6) moves[3] = moves[5] = -1;
    if (colonne == 7) moves[1] = moves[3] = moves[5] = moves[7] = -1;

    return moves;
}

/* Retourne un tableau contenant les indices des 14 cases des 14 moves possibles pour la tour
Si la valeur est -1, le move est impossible car hors limite*/
int * get_legal_tour(int position, int * moves){
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;//on utilise i pour incrémenter au sein des boucles, il est réinitialisé a chaque fois
    int j = 0; //on utilise j pour incrémenter l'indice du tableau moves[], il n'est pas réinitialisé entre les 4 boucles

    for (i = position-8; i>=0; i= i-8){
        moves[j] = i;
        j++;
    }

    for (i = position+8; i<64; i = i+8){
        moves[j] = i;
        j++;
    }

    i = position+1;
    while (get_ligne(i) == ligne){
        moves[j] = i;
        i++;
        j++;
    }

    i=position-1;
    while (get_ligne(i) == ligne){
        moves[j] = i;
        i--;
        j++;
    }

    while (j<14){
        moves[j] = -1;
        j++;
    }

    return moves;
}

void bouger_cavalier(int position){
    int i;
    
    int * moves = (int *)malloc(sizeof(int)*8); //tableau de 8 car il y a 8 moves possibles pour un cavalier
    moves = get_legal_cavalier(position, moves);

    printf("\n\t~~Moves Possibles~~\n");
    for (i = 0; i<8; i++){
        if (moves[i] == -1){ 
            printf(" ");
        }
        else{
            printf("%d: (%d,%d) ", i, get_colonne(moves[i]), get_ligne(moves[i]));
        }
    }
    printf("\n\n");
}

void bouger_tour(int position){
    int i;
    
    int * moves = (int *)malloc(sizeof(int)*14); //tableau de 14 car il y a 14 moves possibles pour une tour
    moves = get_legal_tour(position, moves);

    printf("\n\t~~Moves Possibles~~\n");
    for (i = 0; i<14; i++){
        if (moves[i] == -1){ 
            printf("%d) (-1, -1)", i);
        }
        else{
            printf("%d: (%d,%d) ", i, get_colonne(moves[i]), get_ligne(moves[i]));
        }
    }
    printf("\n\n");
}

void bouger(int position){
    if (echequier[position] == VIDE){
        printf("\nAucune piece selectionnee\n");
        return;
    }

    int i, taille;
    int * moves;
    printf("\n\t**Piece Selectionnee: ");
    print_name(echequier[position]);

    switch (echequier[position]){
        case PION:
            taille = -1;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_tour(position, moves);
            break;
        case CAVALIER:
            taille = 8;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_cavalier(position, moves);
            break;
        case FOU:
            taille = -1;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_tour(position, moves);
            break;
        case TOUR:
            taille = 14;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_tour(position, moves);
            break;
        case REINE:
            taille = -1;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_tour(position, moves);
            break;
        case ROI:
            taille = -1;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_tour(position, moves);
            break;
        default:
            printf("ERREUR");
    }

    printf("\n\t~~Moves Possibles~~\n");
    for (i = 0; i<taille; i++){
        if (moves[i] == -1){ 
            printf("");
        }
        else{
            printf("%d: (%d,%d) ", i, get_colonne(moves[i]), get_ligne(moves[i]));
        }
    }
    printf("\n");
}