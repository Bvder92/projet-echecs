#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// retourne la case du tableau correspondante en fonction de ligne et colonne
int get_pos(int ligne, int colonne)
{
    int i = (ligne)*8;
    return i + (colonne);
}

void affichage_tableau(char *tab, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        printf("%c | ", tab[i]);
    }
}

char int_to_piece(int position)
{

    char c;

    switch (echequier[position])
    {
    case VIDE:
        c = '0';
        break;
    case PION:
        c = 'p';
        break;
    case CAVALIER:
        c = 'n';
        break;
    case FOU:
        c = 'b';
        break;
    case TOUR:
        c = 'r';
        break;
    case REINE:
        c = 'q';
        break;
    case ROI:
        c = 'k';
        break;
    case PION + NOIR:
        c = 'P';
        break;
    case CAVALIER + NOIR:
        c = 'N';
        break;
    case FOU + NOIR:
        c = 'B';
        break;
    case TOUR + NOIR:
        c = 'R';
        break;
    case REINE + NOIR:
        c = 'Q';
        break;
    case ROI + NOIR:
        c = 'K';
        break;
    default:
        c = '?';
    }
    return c;
}

// affiche l'echequier dans son état actuel
void affichage_echequier()
{
    int c = 0;
    printf("     | ~1~ | ~2~ | ~2~ | ~3~ | ~4~ | ~5~ | ~6~ | ~7~ |\n");
    printf("+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
    for (int i = 0; i < MAX; i++)
    {
        if (i % 8 == 0)
        {
            printf("|~%d~ | ", i / 8);
        }
        printf(" %c  | ", int_to_piece(i));
        c++;
        if (c == 8)
        {
            printf("\n+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
            c = 0;
        }
    }
}

// met tout à 0, on verra plus tard pour les castle
void initialiser_fen(FEN fen)
{
    int i;
    for (i = 0; i < 64; i++)
    {
        fen.tab[i] = echequier[i];
    }
    fen.tour = 0;
    fen.castlew = fen.castleb = NULL;
    fen.half_move = 0;
    fen.full_move = 0;
    fen.echec_blanc = 0;
    fen.echec_noir = 0;
}

// place toutes les pièces au bon endroit + initialise le fen
void initialiser_jeu(FEN fen)
{
    int i;
    echequier[0] = TOUR + NOIR;
    echequier[1] = CAVALIER + NOIR;
    echequier[2] = FOU + NOIR;
    echequier[3] = REINE + NOIR;
    echequier[4] = ROI + NOIR;
    echequier[5] = FOU + NOIR;
    echequier[6] = CAVALIER + NOIR;
    echequier[7] = TOUR + NOIR;
    for (i = 8; i < 16; i++)
    {
        echequier[i] = PION + NOIR;
    }
    for (i = 16; i < 48; i++)
    {
        echequier[i] = VIDE;
    }
    for (i = 48; i < 56; i++)
    {
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
    initialiser_fen(fen);
}

// pour placer ce qu'on veut ou on veut
void initialiser_debug()
{
    int i;
    for (i = 0; i < 63; i++)
    {
        echequier[i] = VIDE;
    }
    echequier[50] = PION;
    echequier[33] = PION + NOIR;
    echequier[43] = PION + NOIR;
}

// retourne le numero de ligne d'une pièce en fonction de sa position dans le tableau
int get_ligne(int position)
{
    return (position / 8);
}

// retourne le numero de colonne d'une pièce en fonction de sa position dans le tableau
int get_colonne(int position)
{
    int col;
    col = position - (get_ligne(position) * 8);
    return col;
}

// retourne le nombre de pieces blanches
int compter_blanc()
{
    int c = 0;
    for (int i = 0; i < MAX; i++)
    {
        if (get_color(i) == 0)
        {
            c++;
        }
    }
    return c;
}

// retourne le nombre de pieces noires
int compter_noir()
{
    int c = 0;
    for (int i = 0; i < MAX; i++)
    {
        if (get_color(i) == 1)
        {
            c++;
        }
    }
    return c;
}

// affiche le nom de la piece dont la valeur est passée en argument
void print_name(int position)
{
    int a;
    if (echequier[position] < 128)
    {
        a = 0;
    }
    else
    {
        a = 128;
    }
    switch (echequier[position] - a)
    {
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

void debug_mode()
{
    int ligne, colonne, position, rep = 0;
    int move;
    affichage_echequier(echequier, MAX);
    while (rep != 2)
    {

        printf("\nSelectionner une piece:\n\tColonne: ");
        scanf("%d", &colonne);
        printf("\n\tLigne: ");
        scanf("%d", &ligne);

        position = get_pos(ligne, colonne);

        move = bouger(position); // on vérifie que le move est legit?? et on modifie l'echequier
        if ((move < 64) && (move >= 0))
        {
            echequier[move] = echequier[position];
            echequier[position] = VIDE;
        }
        affichage_echequier(echequier, MAX);
        printf("\n*Continuer? (1: oui, 2: non): ");
        scanf("%d", &rep);
    }
}

void print_color(int position)
{
    if (echequier[position] > 128)
    {
        printf(" noir");
    }
    else if (echequier[position] == VIDE)
    {
        printf("Vide");
    }
    else
    {
        printf(" blanc");
    }
}

// retourne 1 si la piece est noire, 0 si blanc, -1 si vide:
int get_color(int position)
{
    if (echequier[position] > 128)
    {
        return 1;
    }
    else if (echequier[position] == VIDE)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int select_piece(int tour)
{
    int ligne, colonne, position = -1;
    do
    {
        if (tour == 1)
        {
            printf("\nSelectionner une piece Noire:\n\tColonne: ");
        }
        else
        {
            printf("\nSelectionner une piece Blanche:\n\tColonne: ");
        }
        scanf("%d", &colonne);
        printf("\n\tLigne: ");
        scanf("%d", &ligne);

        position = get_pos(ligne, colonne);

        if (get_color(position) != tour)
            printf("\nChoisissez une piece de la bonne couleur!");
    } while (get_color(position) != tour); // check couleur + vide ou pas
    return position;
}

// retourne un nouveau tableau ou les -1 sont à la fin
int *retirer_impossible(int *tab, int taille)
{
    int *tableau_trie = (int *)malloc(sizeof(int) * taille);
    int i = 0, j = 0;
    while (i < taille)
    {
        if (tab[i] != -1)
        {
            tableau_trie[j] = tab[i];
            i++;
            j++;
        }
        else
        {
            i++;
        }
    }
    while (j < taille)
    {
        tableau_trie[j] = -1;
        j++;
    }
    return tableau_trie;
}

/*
MOVES LEGAUX - Les fontions retournent toutes un tableau contenant les indices des cases de l'echequier ou la piece passée en argument peut aller
*/

int *get_lagal_pion_blanc(int position, int *moves)
{
    moves[0] = moves[1] = moves[2] = moves[3] = -1;

    // si la case devant le pion est vide on peut avancer:
    if (echequier[position - 8] == VIDE)
        moves[0] = position - 8;

    // si les deux cases devant le pion sont vide et que le pion n'a jamais bougé on peut double avancer:
    if ((echequier[position - 8] == VIDE) && (echequier[position - 16] == VIDE))
    {
        if (get_ligne(position) == 6)
        {
            moves[1] = position - 16;
        }
    }

    // si la case en haut à droite/gauche contient un pion noir on peut y aller (capture):
    if (echequier[position - 7] != VIDE)
    {
        if (get_color(position) == 1)
        {
            moves[2] = position - 7;
        }
    }

    if (echequier[position - 9] != VIDE)
    {
        if (get_color(position) == 1)
        {
            moves[3] = position - 9;
        }
    }

    return moves;
}

int *get_lagal_pion_noir(int position, int *moves)
{
    moves[0] = moves[1] = moves[2] = moves[3] = -1;

    if (echequier[position + 8] == VIDE)
        moves[0] = position + 8;

    if ((echequier[position + 8] == VIDE) && (echequier[position + 16] == VIDE))
    {
        if (get_ligne(position) == 1)
        {
            moves[1] = position + 16;
        }
    }

    // si la case en bas a droite/gauche contient un pion blanc on peut y aller
    if (echequier[position + 7] != VIDE)
    {
        if (get_color(position) == 0)
        {
            moves[2] = position + 7;
        }
    }

    if (echequier[position + 9] != VIDE)
    {
        if (get_color(position) == 0)
        {
            moves[3] = position + 9;
        }
    }

    return moves;
}

/* Retourne un tableau contenant les moves possibles pour le cavalier, sans les captures
Si la valeur est -1, le move est impossible car hors limite */
int *get_legal_cavalier(int position, int *moves)
{
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;
    // on définit tous les moves théoriquement possibles:
    moves[0] = position - 17;
    moves[1] = position - 15;
    moves[2] = position - 10;
    moves[3] = position - 6;
    moves[4] = position + 6;
    moves[5] = position + 10;
    moves[6] = position + 15;
    moves[7] = position + 17;

    // on élimine les moves impossibles au cas par cas:
    if (ligne == 0)
        moves[0] = moves[1] = moves[2] = moves[3] = -1;
    if (ligne == 1)
        moves[0] = moves[1] = -1;
    if (ligne == 6)
        moves[6] = moves[7] = -1;
    if (ligne == 7)
        moves[4] = moves[5] = moves[6] = moves[7] = -1;

    if (colonne == 0)
        moves[0] = moves[2] = moves[4] = moves[6] = -1;
    if (colonne == 1)
        moves[2] = moves[4] = -1;
    if (colonne == 6)
        moves[3] = moves[5] = -1;
    if (colonne == 7)
        moves[1] = moves[3] = moves[5] = moves[7] = -1;

    return moves; // on retourne le tableau contenant les positions possibles pour le cavalier passé en argument
}

// Retourne un tableau contenant les moves possibles pour la tour, sans les captures
int *get_legal_tour(int position, int *moves, int taille)
{
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;     // on utilise i pour incrémenter au sein des boucles, il est réinitialisé entre chaque while
    int j = 0; // on utilise j pour incrémenter l'indice du tableau moves[], il n'est pas réinitialisé entre les boucles

    for (i = position - 8; i >= 0; i = i - 8)
    { // ligne verticale vers le haut: on retire 8 à i tant qu'il ne sort pas du tableau (i >= 0)
        moves[j] = i;
        j++;
        if (echequier[i] != VIDE)
        {
            break;
        }
    }

    for (i = position + 8; i < 64; i = i + 8)
    { // ligne verticale vers le bas: on ajoute 8 à i tant qu'il ne sort pas du tableau (i < 64)
        moves[j] = i;
        j++;
        if (echequier[i] != VIDE)
        {
            break;
        }
    }

    if (get_colonne(position) != 7)
    {
        i = position + 1; // on initialise i à position+1 et on va l'incrémenter tant qu'il ne change pas de ligne
        while (get_ligne(i) == ligne)
        { // ligne horizontale vers la droite: on ajoute 1 à i tant qu'il ne change pas de ligne
            moves[j] = i;
            i++;
            j++;
            if (echequier[i] != VIDE)
            {
                break;
            }
        }
    }

    if (get_colonne(position) != 0)
    {
        i = position - 1;
        while (get_ligne(i) == ligne)
        { // ligne horizontale vers la gauche: on retire 1 à i tant qu'il ne change pas de ligne
            moves[j] = i;
            i++;
            j++;
            if (echequier[i] != VIDE)
            {
                break;
            }
        }
    }

    while (j < taille)
    {
        moves[j] = -1;
        j++;
    }

    return moves;
}

// retourne un tableau contenant les moves possibles pour le fou, sans captures
int *get_legal_fou(int position, int *moves, int taille)
{
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;
    int j = 0;

    i = position - 9;
    while ((get_colonne(i) < colonne) && (i >= 0))
    { // diagonale vers le haut gauche: on décrémente de 9 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i - 9;
        j++;
        if (echequier[i] != VIDE)
        {
            break;
        }
    }
    i = position - 7;
    while ((get_colonne(i) > colonne) && (i >= 0))
    { // diagonale vers le haut droit: on décrémente de 7 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i - 7;
        j++;
        if (echequier[i] != VIDE)
        {
            break;
        }
    }
    i = position + 7;
    while ((get_colonne(i) < colonne) && (i < 64))
    { // diagonale vers le bas droit: on incrémente de 9 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i + 7;
        j++;
        if (echequier[i] != VIDE)
        {
            break;
        }
    }
    i = position + 9;
    while ((get_colonne(i) > colonne) && (i < 64))
    { // diagonale vers le bas gauche: on incrémente de 7 tant qu'on est dans l'échequier
        moves[j] = i;
        i = i + 9;
        j++;
        if (echequier[i] != VIDE)
        {
            break;
        }
    }

    while (j < taille)
    { // on complète le tableau jusqu'a sa taille maximale pour pas qu'on ne lise de valeurs de cases vides dans la fonction bouger
        moves[j] = -1;
        j++;
    }

    return moves;
}

// retourne un tableau contenant les moves légaux pour le roi, sans captures
int *get_legal_roi(int position, int *moves)
{
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;
    moves[0] = position - 9;
    moves[1] = position - 8;
    moves[2] = position - 7;
    moves[3] = position - 1;
    moves[4] = position + 1;
    moves[5] = position + 7;
    moves[6] = position + 8;
    moves[7] = position + 9;

    // elimination des moves qui sortent de l'echequier
    if ((colonne == 0)) // colonne 0 -> on peut pas aller en -9, -1 ou 7
    {
        moves[0] = moves[3] = moves[5] = -1;
    }
    if ((colonne == 7)) // colonne 7 -> on peut pas aller en -7, 1 ou 9
    {
        moves[2] = moves[4] = moves[7] = -1;
    }
    if ((ligne == 0)) // ligne 0 -> on peut pas aller en -9, -8, -7
    {
        moves[0] = moves[1] = moves[2] = -1;
    }
    if ((ligne == 7)) // ligne 7 -> on peut pas aller en 7 8 ou 9
    {
        moves[5] = moves[6] = moves[7] = -1;
    }

    return moves;
}

// fais appel a la fonction du fou et de la tour puis combine les 2 tableaux: faudra que je la réécrive en plus propre la c'est rude mais flemme
int *get_legal_reine(int position, int *moves, int taille)
{

    int i = 0, a = 0, j; // i sert a incrémenter les boucles, j sert a garder l'indice de moves[] entre les boucles et a sert dans la dernière boucle
    int taille_fou = 13, taille_tour = 14; // plus clair que d'écrire 13 et 14 à chaque fois, faut voir pour un define

    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int *moves_fou = (int *)malloc(sizeof(int) * taille_fou);   // tableau qu'on va remplir avec les 13 moves dispo du fou
    int *moves_tour = (int *)malloc(sizeof(int) * taille_tour); // tableau qu'on va remplir avec les 4 moves restants pour la reine

    moves_fou = get_legal_fou(position, moves_fou, taille_fou);
    moves_tour = get_legal_tour(position, moves_tour, taille_tour);

    while (i < taille_fou)
    { // on remplit moves[] avec les valeurs de moves_fou[]
        moves[i] = moves_fou[i];
        i++;
        j = i;
    } // on a remplit moves avec moves_fou et on a conservé l'indice de la case suivante de moves[]: j

    while (a < taille_tour)
    { // on remplit la suite de moves[](a partir de j) avec les valeurs de moves_tour[](a partir de a = 0)
        moves[j] = moves_tour[a];
        j++;
        a++;
    }

    while (j < taille)
    { // on remplit la fin de moves[](a partir de j) avec des -1 pour ne pas lire de valeurs de cases vides dans la fonction bouger()
        moves[j] = -1;
        j++;
    }

    free(moves_fou);
    free(moves_tour);
    moves_fou = moves_tour = NULL;
    // moves est maintenant composé des moves du fou, puis de ceux de la tour, puis de -1 jusqu'a taille
    return moves;
}

// détecte quelle pièce est présente dans la case "position" et appelle la fonction correspondante pour la déplacer puis retourne le déplacement choisi par l'utilisateur
int bouger(int position)
{
    /* ********************************************************************
    DECLARATIONS ET AFFICHAGE BASIQUE:
    **********************************************************************/
    int i = 0, taille, rep = -1;
    int capture = 0;
    int *moves;
    printf("\n\t**Piece Selectionnee: ");
    print_name(position);
    print_color(position);
    /* ********************************************************************
    ALLOCATION DU TABLEAU DE MOVES SELON LA PIECE CHOISIE
    *********************************************************************/
    switch (echequier[position])
    {
    case PION:
        taille = 4;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_lagal_pion_blanc(position, moves);
        break;
    case PION + NOIR:
        taille = 4;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_lagal_pion_noir(position, moves);
        break;
    case CAVALIER:
    case CAVALIER + NOIR:
        taille = 8;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_cavalier(position, moves);
        break;
    case FOU:
    case FOU + NOIR:
        taille = 13;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_fou(position, moves, taille);
        break;
    case TOUR:
    case TOUR + NOIR:
        taille = 14;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_tour(position, moves, taille);
        break;
    case REINE:
    case REINE + NOIR:
        taille = 27;
        moves = (int *)malloc(sizeof(int) * taille);
        printf("\n malloc reussie");
        moves = get_legal_reine(position, moves, taille);
        break;
    case ROI:
    case ROI + NOIR:
        taille = 8;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_roi(position, moves);
        break;
    default:
        printf("ERREUR");
    }

    /* ********************************************************************
    ELIMINATION DES MOVES IMPOSSIBLES ET AFFICHAGE:
    *********************************************************************/

    for (i = 0; i < taille; i++)
    {
        if (get_color(moves[i]) == get_color(position))
        { // si une case ou on peut aller est occupée par notre couleur, on l'enleve (on peut pas capturer ses pieces)
            moves[i] = -1;
        }
    }
    moves = retirer_impossible(moves, taille); // on met les -1 a la fin du tableau pour pouvoir proposer que les moves possibles a l'utilisateur
    
    printf("\n");
    for (i = 0; i < taille; i++)
    {
        if (moves[i] == -1)
        {
            printf(" [MOINS 1] ");
        }
        else
        {
            printf(" %d: (%d,%d) ", i, get_colonne(moves[i]), get_ligne(moves[i]));
        }
    }

    /* ******************************************************
    SELECTION DU MOVE PAR L'UTILISATEUR:
    *********************************************************/
    do
    {
        printf("\nChoisir move: ");
        scanf("%d", &rep); // moves[rep] est le move qu'on veut faire
        if ((rep > taille) || (moves[rep] < 0))
        {
            printf("\nMove invalide!");
        }
    } while ((rep > taille) || (moves[rep] < 0));

    // on regarde si le move choisi est une capture ou non
    if (moves[rep] != VIDE)
    {
        if (get_color(moves[rep]) == get_color(position))
        { // ca devrait pas arriver vu qu'on la check juste au dessus
            printf("\nCAPTURE PIECE ALLIEE");
        }
        else
        {
            capture = 1;
        }
    }

    // libération et retour
    rep = moves[rep]; // plus besoin de rep et on doit stocker moves[rep] qlq part avant de free moves[]
    free(moves);
    moves = NULL;
    return rep;
}

// retourne le tableau de moves legaux d'une piece(bouger sans la partie ou on choisit)
int *recuperer_moves(int position)
{

    int *moves;
    int taille, i;

    switch (echequier[position])
    {
    case PION:
        taille = 4;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_lagal_pion_blanc(position, moves);
        break;
    case PION + NOIR:
        taille = 4;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_lagal_pion_noir(position, moves);
        break;
    case CAVALIER:
    case CAVALIER + NOIR:
        taille = 8;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_cavalier(position, moves);
        break;
    case FOU:
    case FOU + NOIR:
        taille = 13;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_fou(position, moves, taille);
        break;
    case TOUR:
    case TOUR + NOIR:
        taille = 14;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_tour(position, moves, taille);
        break;
    case REINE:
    case REINE + NOIR:
        taille = 27;
        moves = (int *)malloc(sizeof(int) * taille);
        printf("\n malloc reussie");
        moves = get_legal_reine(position, moves, taille);
        break;
    case ROI:
    case ROI + NOIR:
        taille = 8;
        moves = (int *)malloc(sizeof(int) * taille);
        moves = get_legal_roi(position, moves);
        break;
    default:
        printf("ERREUR");
    }

    /* ********************************************************************
    ELIMINATION DES MOVES IMPOSSIBLES ET AFFICHAGE:
    *********************************************************************/

    for (i = 0; i < taille; i++)
    {
        if (get_color(moves[i]) == get_color(position))
        { // si une case ou on peut aller est occupée par notre couleur, on l'enleve (on peut pas capturer ses pieces)
            moves[i] = -1;
        }
    }
    moves = retirer_impossible(moves, taille);

    return moves;
}

FEN verifier_echec(FEN fen)
{

    /* ********************************************************************
    DECLARATIONS:
    *********************************************************************/
    int i, j = 0, taille;
    int * moves = NULL; //pour stocker les moves de la piece qu'on va "étudier"
    int *echec_blanc = NULL; // cases de l'echequier contenant une piece pouvant tuer le roi blanc
    int *echec_noir = NULL; // cases de l'echequier contenant une piece pouvant tuer le roi noir
    int compteur_noir = 0;
    int compteur_blanc = 0;

    int taille_blanc = sizeof(int) * compter_noir();
    int taille_noir = sizeof(int) * compter_blanc();

    // tableaux qui vont contenir toutes les pieces pouvants echec le roi => le max théorique est le nombre de pieces ennemies
    echec_blanc = (int *)malloc(taille_blanc);
    echec_noir = (int *)malloc(taille_noir);

    for (i = 0; i < taille_blanc; i++)
    {
        echec_blanc[i] = -1;
    }
    for (i = 0; i < taille_blanc; i++)
    {
        echec_noir[i] = -1;
    }

    /* ********************************************************************
    PARCOURS DE L'ÉCHEQUIER ET STOCKAGE DES VALEURS DANS LES TABLEAUX:
    *********************************************************************/

    for (i = 0; i < MAX; i++) // on parcours tout l'echequier
    {
        if (echequier[i] != VIDE) // si une case contient une piece
        {
            moves = recuperer_moves(i); // on récupere ses moves

            while (j != -1) // on parcours ses moves (jusqu'a -1 car les -1 sont a la fin du tableau moves[])
            {
                if (echequier[moves[j]] == ROI) // si un de ces moves est le roi blanc (la piece qu'on étudie est donc noire)
                {
                    if (get_color(j) != NOIR)
                    {
                        printf("\nprobleme: une piece peut capturer son propore roi\n"); // on sait jamais
                    }
                    echec_blanc[compteur_blanc] = i; // on ajoute cette piece a la liste des pieces pouvant tuer le roi
                    compteur_noir++;
                }

                else if (echequier[moves[j]] == ROI + NOIR) // on va faire la meme chose qu'au dessus
                {
                    if (get_color(j) != 0)
                    {
                        printf("\nprobleme: une piece peut capturer son propore roi\n"); // on sait jamais
                    }
                    echec_noir[compteur_noir] = i;
                    compteur_noir++;
                }
            }
        }
        free(moves);
        moves = NULL;
    }

    /* ********************************************************************
    PARCOURS DES TABLEAUX POUR SAVOIR SI IL Y A ECHEC:
    *********************************************************************/

    for (i = 0; i < taille_blanc; i++)
    {
        if (echec_blanc[i] != -1)
        {
            fen.echec_blanc = 1;
        }
    }

    for (i = 0; i < taille_noir; i++)
    {
        if (echec_noir[i] != -1)
        {
            fen.echec_noir = 1;
        }
    }

    free(echec_blanc);
    free(echec_noir);
    echec_blanc = NULL;
    echec_noir = NULL;
    return fen;
}

FEN update_fen(FEN fen)
{
    int i;
    for (i = 0; i < 64; i++)
    {
        fen.tab[i] = echequier[i];
    }
    fen.half_move++;
    if (fen.tour == 1)
    {
        fen.tour = 0;
        fen.full_move++;
    }
    else
    {
        fen.tour = 1;
    }

    //fen = verifier_echec(fen); //modifie les valeurs de echec_blanc et echec_noir

    return fen;
}