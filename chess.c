#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//inutilisée
void init_echequier(){
    for (int i = 0; i<MAX; i++){
        echequier[i] = '0';
    }
}

//inutilisée
void placer_pion(int ligne, int colonne){
    int i = (ligne -1)*8;
    i = i + colonne-1;
    echequier[i] = '1';
}

//retourne la case du tableau correspondante en fonction de ligne et colonne
int get_pos(int ligne, int colonne){
    int i = (ligne)*8;
    return i + (colonne);
}

void affichage_tableau(char * tab, int taille){
    for (int i = 0; i<taille; i++){
        printf("%c | ", tab[i]);
    }
}

//affiche l'echequier dans son état actuel
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

//place toutes les pièces au bon endroit
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

//pour placer ce qu'on veut ou on veut
void initialiser_debug(){
    int i;
    for (i = 0; i < 63; i++){
        echequier[i] = VIDE;
    }
    echequier[3] = REINE;
}

//retourne le numero de ligne d'une pièce en fonction de sa position dans le tableau
int get_ligne(int position){
    return (position/8);
}

//retourne le numero de colonne d'une pièce en fonction de sa position dans le tableau
int get_colonne(int position){
    int col;
    col = position - (get_ligne(position)*8);
    return col;
}

//affiche le nom de la piece dont la valeur est passée en argument
void print_name(int position){
    switch (echequier[position]){
        case VIDE: 
            printf("Vide");
            break;
        case PION:
            printf("Pion");
            break;
        case CAVALIER:
            printf("Cavalier");
            break;
        case FOU:
            printf("Fou");
            break;
        case TOUR:
            printf("Tour");
            break;
        case REINE:
            printf("Reine");
            break;
        case ROI:
            printf("Roi");
            break;
        default:
            printf("ERREUR");
    }
}

//retourne un nouveau tableau ou les -1 sont à la fin
int * retirer_impossible(int *tab, int taille)
{
    int * tableau_trie = (int *)malloc(sizeof(int)*taille);
    int i = 0, j = 0;
    while (i < taille){
        if (tab[i] != -1){
            tableau_trie[j] = tab[i];
            i++;
            j++;
        }
        else{
            i++;
        }
    }
    while(j <taille){
        tableau_trie[j] = -1;
        j++;
    }
    return tableau_trie;
}

/*
-> Pour l'instant la fonction affiche les coordonnées des mouvements légaux sous forme d'indice du tableau echequier
Inutilisée mais on garde pour quand il faudra get_legal_pion()
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
    //on définit tous les moves théoriquement possibles:
    moves[0] = position - 17;
    moves[1] = position - 15;
    moves[2] = position - 10;
    moves[3] = position - 6;
    moves[4] = position + 6;
    moves[5] = position + 10;
    moves[6] = position + 15;
    moves[7] = position + 17;

    //on élimine les moves impossibles au cas par cas:
    if (ligne == 0) moves[0] = moves[1] = moves[2] = moves[3] = -1;
    if (ligne == 1) moves[0] = moves[1] = -1;
    if (ligne == 6) moves[6] = moves[7] = -1;
    if (ligne == 7) moves[4] = moves[5] = moves[6] = moves[7] = -1;

    if (colonne == 0) moves[0] = moves[2] = moves[4] = moves[6] = -1;
    if (colonne == 1) moves[2] = moves[4] = -1;
    if (colonne == 6) moves[3] = moves[5] = -1;
    if (colonne == 7) moves[1] = moves[3] = moves[5] = moves[7] = -1;

    return moves; //on retourne le tableau contenant les positions possibles pour le cavalier passé en argument
}

/* Retourne un tableau contenant les indices des 14 cases des 14 moves possibles pour la tour
Si la valeur est -1, le move est impossible car hors limite*/
int * get_legal_tour(int position, int * moves, int taille){
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;//on utilise i pour incrémenter au sein des boucles, il est réinitialisé entre chaque while
    int j = 0; //on utilise j pour incrémenter l'indice du tableau moves[], il n'est pas réinitialisé entre les boucles

    for (i = position-8; i>=0; i= i-8){ //ligne verticale vers le haut: on retire 8 à i tant qu'il ne sort pas du tableau (i >= 0)
        moves[j] = i;
        j++;
    }

    for (i = position+8; i<64; i = i+8){ //ligne verticale vers le bas: on ajoute 8 à i tant qu'il ne sort pas du tableau (i < 64)
        moves[j] = i;
        j++;
    }

    i = position+1; //on initialise i à position+1 et on va l'incrémenter tant qu'il ne change pas de ligne
    while (get_ligne(i) == ligne){ //ligne horizontale vers la droite: on ajoute 1 à i tant qu'il ne change pas de ligne 
        moves[j] = i;
        i++;
        j++;
    }

    i=position-1;
    while (get_ligne(i) == ligne){ //ligne horizontale vers la gauche: on retire 1 à i tant qu'il ne change pas de ligne
        moves[j] = i;
        i--;
        j++;
    }

    return moves;
}

int * get_legal_fou(int position, int * moves, int taille){
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;
    int j = 0;
    
    i = position-9;
    while((get_colonne(i) < colonne) && (i>=0)){ //diagonale vers le haut gauche: on décrémente de 9 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i-9;
        j++;
    }
    i = position-7;
    while((get_colonne(i) > colonne) && (i>=0)){ //diagonale vers le haut droit: on décrémente de 7 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i-7;
        j++;
    }
    i = position+7;
    while((get_colonne(i) < colonne) && (i<64)){ //diagonale vers le bas droit: on incrémente de 9 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i+7;
        j++;
    }
    i = position+9;
    while((get_colonne(i) > colonne) && (i<64)){ //diagonale vers le bas gauche: on incrémente de 7 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i+9;
        j++;
    }
    
    while (j<taille){ //on complète le tableau jusqu'a sa taille maximale pour pas qu'on ne lise de valeurs de cases vides dans la fonction bouger
        moves[j] = -1;
        j++;
    }

    return moves;
}

int * get_legal_roi(int position, int * moves){
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    moves[0] = position - 8; 
    moves[1] = position - 1;
    moves[2] = position + 1;
    moves[3] = position + 8;

    if (ligne == 0) moves[0] = -1;
    if (colonne == 0) moves[1] = -1;
    if (colonne == 7) moves[2] = -1;
    if (ligne == 7) moves[3] = -1;

    return moves;
}

//fais appel a la fonction du fou et de la tour puis combine les 2 tableaux: faudra que je la réécrive en plus propre la c'est rude mais flemme
int * get_legal_reine(int position, int * moves, int taille){

    int i = 0, a = 0, j; //i sert a incrémenter les boucles, j sert a garder l'indice de moves[] entre les boucles et a sert dans la dernière boucle
    int taille_fou = 13, taille_tour = 14; //plus clair que d'écrire 13 et 14 à chaque fois, faut voir pour un define
    
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int * moves_fou = (int *)malloc(sizeof(int)*taille_fou); //tableau qu'on va remplir avec les 13 moves dispo du fou
    int * moves_tour = (int *)malloc(sizeof(int)*taille_tour); //tableau qu'on va remplir avec les 4 moves restants pour la reine
    
    moves_fou = get_legal_fou(position, moves_fou, taille_fou);
    moves_tour = get_legal_tour(position, moves_tour, taille_tour);

    while (i < taille_fou){//on remplit moves[] avec les valeurs de moves_fou[]
        moves[i] = moves_fou[i];
        i++;
        j = i;
    } //on a remplit moves avec moves_fou et on a conservé l'indice de la case suivante de moves[]: j

    while (a < taille_tour){ //on remplit la suite de moves[](a partir de j) avec les valeurs de moves_tour[](a partir de a = 0)
        moves[j] = moves_tour[a];
        j++;
        a++;
    }

    while (j < taille){ //on remplit la fin de moves[](a partir de j) avec des -1 pour ne pas lire de valeurs de cases vides dans la fonction bouger()
        moves[j] = -1;
        j++;
    }

    free(moves_fou);
    free(moves_tour);
    //moves est maintenant composé des moves du fou, puis de ceux de la tour, puis de -1 jusqu'a taille
    return moves;
}

//détecte quelle pièce est présente dans la case "position" et appelle la fonction correspondante pour la déplacer puis retourne le déplacement choisi par l'utilisateur
int bouger(int position){
    printf("\nCa debug a l'ancienne, entree dans la fonction bouger\n");
    if (echequier[position] == VIDE){
        printf("\nAucune piece selectionnee\n");
        return -1;
    }

    int i = 0, taille, rep = -1;
    int * moves;
    printf("\n\t**Piece Selectionnee: ");
    print_name(position);

    switch (echequier[position]){
        case PION:
            taille = -1;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_tour(position, moves, taille);
            break;
        case CAVALIER:
            taille = 8;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_cavalier(position, moves);
            break;
        case FOU:
            taille = 13;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_fou(position, moves, taille);
            break;
        case TOUR:
            taille = 14;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_tour(position, moves, taille);
            break;
        case REINE:
            taille = 27;
            printf("\nAvant malloc");
            moves = (int *)malloc(sizeof(int)*taille);
            printf("\n malloc reussie");
            moves = get_legal_reine(position, moves, taille);
            printf("\nget_legal reussie");
            break;
        case ROI:
            taille = 4;
            moves = (int *)malloc(sizeof(int)*taille);
            moves = get_legal_roi(position, moves);
            break;
        default:
            printf("ERREUR");
    }

    //moves = retirer_impossible(moves, taille); //on met les -1 a la fin du tableau pour pouvoir proposer que les moves possibles a l'utilisateur

    printf("\n\t~~Moves Possibles~~\n");
    for (i = 0; i<taille; i++){
        if (moves[i] == -1){ 
            printf("[-MOINS 1] ");
        }
        else{
            printf("%d: (%d,%d) ", i, get_colonne(moves[i]), get_ligne(moves[i]));
        }
    }
    printf("\n");

    /*while (moves[i] != -1){
        printf("%d: (%d,%d) ", i, get_colonne(moves[i]), get_ligne(moves[i]));
        i++;
    }*/
    
    //PAS FINI
    do
    {
        printf("\nChoisir move: ");
        scanf("%d", &rep);
    } while ((rep > taille) || (moves[rep] < 0));
    
    rep = moves[rep];
    free(moves);
    return rep;
}