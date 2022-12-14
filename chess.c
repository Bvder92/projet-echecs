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

// affiche l'initiale d'une piece, utilisée pour affichage_echequier
char int_to_piece(int position)
{
    char c;
    switch (echequier[position])
    {
    case VIDE:
        c = '.';
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
    case ROI + SPECIAL:
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
    case ROI + NOIR + SPECIAL:
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
    printf("     | ~0~ | ~1~ | ~2~ | ~3~ | ~4~ | ~5~ | ~6~ | ~7~ |\n");
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
FEN initialiser_fen(FEN fen)
{
    //int i;
    /*for (i = 0; i < 64; i++)
    {
        fen.tab[i] = echequier[i];
    }*/
    fen.tour = 0;
    fen.castlew = fen.castleb = NULL;
    fen.half_move = 0;
    fen.full_move = 0;
    fen.echec = -1;
    fen.echec_et_mat = -1;
    return fen;
}

// place toutes les pièces au bon endroit + initialise le fen
void initialiser_jeu()
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
}

// pour placer ce qu'on veut ou on veut
void initialiser_debug()
{
    int i;
    for (i = 0; i < 63; i++)
    {
        echequier[i] = VIDE;
    }
    echequier[50] = PION + NOIR;
    // echequier[51] = PION+NOIR;
}

// retourne le numero de ligne d'une pièce en fonction de sa position dans le tableau
int get_ligne(int position)
{
    return (position / 8);
}

// retourne le numero de colonne d'une pièce en fonction de sa position dans le tableau
int get_colonne(int position)
{
    return (position - (get_ligne(position) * 8));
}

// copie le plateau dans un tableau passé en argument
int *copie_echequier(int * plateau, int *tab)
{
    for (int i = 0; i < MAX; i++)
    {
        tab[i] = plateau[i];
    }
    return tab;
}

// retourne le nombre de pieces blanches
int compter_blanc()
{
    int c = 0;
    for (int i = 0; i < MAX; i++)
    {
        if (get_color(i, echequier) == 0)
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
        if (get_color(i, echequier) == 1)
        {
            c++;
        }
    }
    return c;
}

// retourne le nombre de pièces de "couleur"
int compter(int couleur, int * plateau){
    int c = 0;
    int i = 0;
    while (i < MAX && c < 16){
        if (get_color(i, plateau) == couleur){
            c++;
        }
        i++;
    }
    return c;
}

// affiche le nom de la piece dont la valeur est passée en argument
void print_name(int position) // pas besoin de créer une variable a tu peuc juste changer le case
{
    switch (echequier[position])
    {
    case VIDE:
        printf("Vide");
        break;
    case PION:
    case PION + NOIR:
        printf("Pion");
        break;
    case CAVALIER:
    case CAVALIER + NOIR:
        printf("Cavalier");
        break;
    case FOU:
    case FOU + NOIR:
        printf("Fou");
        break;
    case TOUR:
    case TOUR + NOIR:
        printf("Tour");
        break;
    case REINE:
    case REINE + NOIR:
        printf("Reine");
        break;
    case ROI:
    case ROI + SPECIAL:
    case ROI + NOIR + SPECIAL:
    case ROI + NOIR:
        printf("Roi");
        break;
    default:
        printf("ERREUR");
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

// retourne 1 pour noir, 0 pour blanc, -1 pour vide
int get_color(int position, int *tab)
{
    if (tab[position] > 128)
    {
        return 1;
    }
    else if (tab[position] == VIDE)
    {
        return -1;
    }
    else
        return 0;
}

// retourne le nombre de valeurs d'un tableau de moves (sans les -1 du coup)
int get_nombre_moves(int *moves, int taille)
{
    int nombre_moves = 0;
    for (int i = 0; i < taille; i++)
    {
        if (moves[i] != -1)
        {
            nombre_moves++;
        }
    }
    return nombre_moves;
}

int get_pos_roi(int couleur)
{
    int i = 0;
    int pos_roi;
    if (couleur == 1)
    {
        while (i < MAX)
        {
            if (echequier[i] == ROI + NOIR)
            {
                pos_roi = i;
            }
            i++;
        }
    }
    else if (couleur == 0)
    {
        while (i < MAX)
        {
            if (echequier[i] == ROI)
            {
                pos_roi = i;
            }
            i++;
        }
    }
    else
    {
        return -1;
    }
    return pos_roi;
}

// version alternative du jeu avec des printf etc
void debug_mode()
{
    int ligne, colonne, position, rep = 0;
    int move;
    FEN fen;
    initialiser_debug();
    initialiser_fen(fen);
    affichage_echequier(echequier, MAX);
    while (rep < 10 )//fen.echec == -1)
    {

        printf("\nSelectionner une piece:\n\tColonne: ");
        scanf("%d", &colonne);
        printf("\n\tLigne: ");
        scanf("%d", &ligne);

        position = get_pos(ligne, colonne);

        move = bouger(position, fen); // on vérifie que le move est legit?? et on modifie l'echequier
        if ((move < 64) && (move >= 0))
        {
            echequier[move] = echequier[position];
            echequier[position] = VIDE;
        }
        affichage_echequier(echequier, MAX);
        printf("\n*Continuer? (1: oui, 2: non): ");
        scanf("%d", &rep);
        rep++;
    }
}

// demande a l'utilisateur de selectionner une piece de la bonne couleur
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

        if (get_color(position, echequier) != tour)
        {
            printf("\nChoisissez une piece de la bonne couleur!");
        }
    } while (get_color(position, echequier) != tour); // check couleur + vide ou pas
    return position;
}

// verifie si la piece selectionee par l'utilisateur peut bouger
int select_piece_mieux(int couleur)
{
    int i, taille_liste, valide = 0;
    int *liste_pieces;

    if (couleur == 0)
    {
        taille_liste = compter_blanc();
    }
    else
    {
        taille_liste = compter_noir();
    }

    liste_pieces = (int *)malloc(sizeof(int) * taille_liste);
    if (liste_pieces == NULL)
    {
        fprintf(stderr, "\nSELECT PIECE: ECHEC MALLOC LISTE_PIECES\n");
        return -1;
    }
    liste_pieces = liste_moves(couleur, liste_pieces, taille_liste, echequier);

    int position = select_piece(couleur);

    for (i = 0; i < taille_liste; i++)
    {
        if (liste_pieces[i] == position)
        {
            valide = 1;
            break;
        }
    }

    if (valide == 1)
    {
        return position;
    }
    return -1;
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

int *get_legal_pion_blanc(int position, int *moves, int *tab)
{
    moves[0] = moves[1] = moves[2] = moves[3] = -1;

    // si la case devant le pion est vide on peut avancer:
    if (tab[position - 8] == VIDE)
        moves[0] = position - 8;

    // si les deux cases devant le pion sont vide et que le pion n'a jamais bougé on peut double avancer:
    if ((tab[position - 8] == VIDE) && (tab[position - 16] == VIDE))
    {
        if (get_ligne(position) == 6)
        {
            moves[1] = position - 16;
        }
    }

    // si la case en haut à droite/gauche contient un pion noir on peut y aller (capture):
    if ((get_colonne(position) != 7) && (get_color(position - 7, tab) == 1))
    {
        moves[2] = position - 7;
    }

    if ((get_colonne(position) != 0) && (get_color(position - 9, tab) == 1))
    {
        moves[3] = position - 9;
    }

    return moves;
}

int *get_legal_pion_noir(int position, int *moves, int *tab)
{
    moves[0] = moves[1] = moves[2] = moves[3] = -1;

    if (tab[position + 8] == VIDE)
        moves[0] = position + 8;

    if ((tab[position + 8] == VIDE) && (tab[position + 16] == VIDE))
    {
        if (get_ligne(position) == 1)
        {
            moves[1] = position + 16;
        }
    }

    // si la case en bas a droite/gauche contient un pion blanc on peut y aller

    if ((get_colonne(position) != 0) && (get_color(position + 7, tab) == 0))
    {
        moves[2] = position + 7;
    }

    if ((get_colonne(position) != 7) && (get_color(position + 9, tab) == 0))
    {
        moves[3] = position + 9;
    }

    return moves;
}

/* Retourne un tableau contenant les moves possibles pour le cavalier, sans les captures
Si la valeur est -1, le move est impossible car hors limite */
int *get_legal_cavalier(int position, int *moves, int *tab)
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
int *get_legal_tour(int position, int *moves, int taille, int *tab)
{
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;     // on utilise i pour incrémenter au sein des boucles, il est réinitialisé entre chaque while
    int j = 0; // on utilise j pour incrémenter l'indice du tableau moves[], il n'est pas réinitialisé entre les boucles

    for (i = position - 8; i >= 0; i = i - 8)
    { // ligne verticale vers le haut: on retire 8 à i tant qu'il ne sort pas du tableau (i >= 0)
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
    }

    for (i = position + 8; i < 64; i = i + 8)
    { // ligne verticale vers le bas: on ajoute 8 à i tant qu'il ne sort pas du tableau (i < 64)
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
    }

    i = position + 1; // on initialise i à position+1 et on va l'incrémenter tant qu'il ne change pas de ligne
    while ((get_ligne(i) == ligne) && (i < 64) && (i >= 0))
    { // ligne horizontale vers la droite: on ajoute 1 à i tant qu'il ne change pas de ligne
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
        i++;
    }

    i = position - 1;
    while ((get_ligne(i) == ligne) && (i < 64) && (i >= 0))
    { // ligne horizontale vers la gauche: on retire 1 à i tant qu'il ne change pas de ligne
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
        i--;
    }

    while (j < taille)
    {
        moves[j] = -1;
        j++;
    }

    return moves;
}

// retourne un tableau contenant les moves possibles pour le fou, sans captures
int *get_legal_fou(int position, int *moves, int taille, int *tab)
{
    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int i;
    int j = 0;

    i = position - 9;
    while ((get_colonne(i) < colonne) && (i >= 0))
    { // diagonale vers le haut gauche: on décrémente de 9 tant qu'on est dans l'échequier
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
        i = i - 9;
    }
    i = position - 7;
    while ((get_colonne(i) > colonne) && (i >= 0))
    { // diagonale vers le haut droit: on décrémente de 7 tant qu'on est dans l'échequier
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
        i = i - 7;
    }
    i = position + 7;
    while ((get_colonne(i) < colonne) && (i < 64))
    { // diagonale vers le bas droit: on incrémente de 9 tant qu'on est dans l'échequier
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
        i = i + 7;
    }
    i = position + 9;
    while ((get_colonne(i) > colonne) && (i < 64))
    { // diagonale vers le bas gauche: on incrémente de 7 tant qu'on est dans l'échequier
        moves[j] = i;
        j++;
        if (tab[i] != VIDE)
        {
            break;
        }
        i = i + 9;
    }

    while (j < taille)
    { // on complète le tableau jusqu'a sa taille maximale pour pas qu'on ne lise de valeurs de cases vides dans la fonction bouger
        moves[j] = -1;
        j++;
    }

    return moves;
}

// retourne un tableau contenant les moves légaux pour le roi, sans captures
int *get_legal_roi(int position, int *moves, int *tab)
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
    moves[8] = -1;
    moves[9] = -1;

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
int *get_legal_reine(int position, int *moves, int taille, int *tab)
{

    int i = 0, a = 0, j;                   // i sert a incrémenter les boucles, j sert a garder l'indice de moves[] entre les boucles et a sert dans la dernière boucle
    int taille_fou = 13, taille_tour = 14; // plus clair que d'écrire 13 et 14 à chaque fois, faut voir pour un define

    int ligne = get_ligne(position);
    int colonne = get_colonne(position);
    int *moves_fou = (int *)malloc(sizeof(int) * taille_fou);   // tableau qu'on va remplir avec les 13 moves dispo du fou
    int *moves_tour = (int *)malloc(sizeof(int) * taille_tour); // tableau qu'on va remplir avec les 4 moves restants pour la reine

    moves_fou = get_legal_fou(position, moves_fou, taille_fou, tab);
    moves_tour = get_legal_tour(position, moves_tour, taille_tour, tab);

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

// retroune la taille du tableau de moves de la piece dans echequier[position]:
int get_taille_moves(int position, int *tab)
{
    int taille;
    switch (tab[position])
    {
    case PION:
    case PION + SPECIAL:
    case PION + NOIR:
    case PION + NOIR + SPECIAL:
        taille = 4;
        break;
    case CAVALIER:
    case CAVALIER + NOIR:
        taille = 8;
        break;
    case ROI:
    case ROI + SPECIAL:
    case ROI + NOIR + SPECIAL:
    case ROI + NOIR:
        taille = 10;
        break;
    case FOU:
    case FOU + NOIR:
        taille = 13;
        break;
    case TOUR:
    case TOUR + NOIR:
        taille = 14;
        break;
    case REINE:
    case REINE + NOIR:
        taille = 27;
        break;
    default:
        fprintf(stderr, "ERREUR GET_TAILLE_MOVES: PIECE INCONNUE, piece = %d\n");
        return 0;
    }
    return taille;
}

// retourne le tableau de moves legaux d'une piece(bouger sans la partie ou on choisit)
int *recuperer_moves(int position, int taille, int *tab)
{

    int *moves;
    int i;
    moves = (int *)malloc(sizeof(int) * taille);

    switch (tab[position])
    {
    case PION:
    case PION + SPECIAL:
        moves = get_legal_pion_blanc(position, moves, tab);
        break;
    case PION + NOIR:
    case PION + NOIR + SPECIAL:
        moves = get_legal_pion_noir(position, moves, tab);
        break;
    case CAVALIER:
    case CAVALIER + NOIR:
        moves = get_legal_cavalier(position, moves, tab);
        break;
    case FOU:
    case FOU + NOIR:
        moves = get_legal_fou(position, moves, taille, tab);
        break;
    case TOUR:
    case TOUR + NOIR:
        moves = get_legal_tour(position, moves, taille, tab);
        break;
    case REINE:
    case REINE + NOIR:
        moves = get_legal_reine(position, moves, taille, tab);
        break;
    case ROI:
    case ROI + SPECIAL:
        moves = get_legal_roi(position, moves, tab);
        break;
    case ROI + NOIR:
    case ROI + NOIR + SPECIAL:
        moves = get_legal_roi(position, moves, tab);
        break;
    default:
        fprintf(stderr, "ERREUR RECUP MOVES: PIECE INCONNUE\n");
        return NULL;
    }

    if (moves == NULL)
    {
        fprintf(stderr, "Fonction recuperer_moves: erreur malloc");
        return NULL;
    }

    /* ********************************************************************
    ELIMINATION DES MOVES IMPOSSIBLES:
    *********************************************************************/

    for (i = 0; i < taille; i++)
    {
        if (get_color(moves[i], tab) == get_color(position, tab))
        { // si une case ou on peut aller est occupée par notre couleur, on l'enleve (on peut pas capturer ses pieces)
            moves[i] = -1;
        }
    }
    moves = retirer_impossible(moves, taille);
    return moves;
}

// ajoute les 2 castle a la fin du tableau de moves: moves[8] = côté reine, moves[9] = côté roi
int *ajouter_castle(int couleur, int *moves, int * castleb, int * castlew)
{

    if (couleur == 1)
    {
        if (castleb[0] == 1)
        {
            moves[8] = 2;
        }
        if (castleb[1] == 1)
        {
            moves[9] = 6;
        }
    }
    else if (couleur == 0)
    {
        if (castlew[0] == 1)
        {
            moves[8] = 58;
        }
        if (castlew[1] == 1)
        {
            moves[9] = 62;
        }
    }
    else
    {
        fprintf(stderr, "ERREUR FONCTION AJOUT CASTLE");
    }
    return moves;
}

// retire tous les moves qui vont entrainer un echec
int *retirer_echec(int position, int *moves, int * plateau)
{
    int j, couleur, taille_moves;
    int *echequier_tmp;

    echequier_tmp = (int *)malloc(sizeof(int) * MAX);

    couleur = get_color(position, echequier);
    taille_moves = get_taille_moves(position, echequier);

    j = 0;
    while (moves[j] != -1 && j < taille_moves) // on va effectuer chaque move dans un echequier temporaire et regarder si le move crée un echec
    {
        echequier_tmp = copie_echequier(plateau, echequier_tmp);
        echequier_tmp[moves[j]] = echequier_tmp[position];
        echequier_tmp[position] = VIDE;

        if (verifier_echec(echequier_tmp) == couleur)
        {
            moves[j] = -1;
        }
        j++;
    }
    free(echequier_tmp);
    echequier_tmp = NULL;

    return moves;
}

// retourne un tableau contenant la position de toutes les pieces pouvant bouger: si il n'y en a aucune, echec et mat
int *liste_moves(int couleur, int *liste_pieces, int taille_liste, int * plateau)
{

    int i, j = 0, taille_moves;
    int *moves; // liste = liste de pieces qu'on va retourner, moves = moves de chaque piece étudiée

    // on parcours tout l'echequier:
    for (i = 0; i < MAX; i++)
    {
        // on regarde les moves dispo pour chaque piece alliée qu'on trouve
        if ((plateau[i] != VIDE) && (get_color(i, plateau) == couleur))
        {

            taille_moves = get_taille_moves(i, plateau);
            moves = (int *)malloc(sizeof(int) * taille_moves);
            if (moves == NULL)
            {
                fprintf(stderr, "\nFONTION LISTE_MOVES: ECHEC MALLOC MOVES\n");
                return NULL;
            }
            moves = recuperer_moves(i, taille_moves, plateau);

            if (get_nombre_moves(moves, taille_moves) != 0) // si la piece en position i peut bouger
            {
                moves = retirer_echec(i, moves, plateau);                // on verifie qu'elle puisse bouger sans generer d'echec
                if (get_nombre_moves(moves, taille_moves) != 0) // si oui
                {
                    liste_pieces[j] = i; // on ajoute la piece de la case i a la liste de pieces pouvant bouger
                    j++;
                }
            }
        }
        free(moves);
        moves = NULL;
    }

    // on remplit le reste de la liste avec des -1:
    while (j < taille_liste)
    {
        liste_pieces[j] = -1;
        j++;
    }
    return liste_pieces;
}

// retourne le tableau de moves trié d'une piece, avec castle et sans echecs:
int *get_moves_total(int *moves, int taille, int position, int * castleb, int * castlew, int * plateau)
{

    moves = recuperer_moves(position, taille, plateau);
    moves = retirer_echec(position, moves, plateau);
    if (plateau[position] == ROI || plateau[position] == ROI + NOIR)
    {
        moves = ajouter_castle(get_color(position, plateau), moves, castleb, castlew);
    }
    moves = retirer_impossible(moves, taille);

    return moves;
}

int bouger(int position, FEN fen)
{
    printf("\n\t**Piece Selectionnee: ");
    print_name(position);
    print_color(position);

    /* ******************************************************
    DELARATIONS ET RECUPERATION DES MOVES:
    *********************************************************/

    int i, taille, rep = -1;
    int capture = 0;
    int couleur = get_color(position, echequier);
    int *moves;

    taille = get_taille_moves(position, echequier);
    moves = (int *)malloc(sizeof(int) * taille);
    if (moves == NULL)
    {
        fprintf(stderr, "erreur malloc bouger_alt");
        return 0;
    }

    moves = get_moves_total(moves, taille, position, fen.castleb, fen.castlew, echequier);

    /*int * scores = (int *)malloc(sizeof(int)*taille);
    scores = get_score_all_moves(position, fen);
    if (scores == NULL){
        fprintf(stderr, "SCORES NULL");
        return 0;
    }*/

    /* ******************************************************
    AFFICHAGE:
    *********************************************************/

    printf("\n");

    i = 0;
    while (i < taille && moves[i] != -1)
    {
        printf("%d: (%d,%d) ", i, get_colonne(moves[i]), get_ligne(moves[i]));
        i++;
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


    // on regarde si le move permet une promotion ou non
    if(echequier[position] == PION || echequier[position] == PION + NOIR){
        if( (moves[rep] >= 0 && moves[rep] <= 7) || (moves[rep] >= 56 && moves[rep] <= 63) ){ //si le mouvement de l'utilisateur est un pion allant dans la derniere ligne 
            int promo; 
            printf("\nVOUS ETES EN SITUATION DE PROMOTION, VEUILLEZ SELECTIONNER UNE PIECE PARMIS = 1 : DAME, 2 : TOUR, 3 : FOU, 4 : CAVALIER\n");
            do{
                printf("Veuillez choisir une piece : ");
                scanf("%d", &promo);
                if(promo != 1 && promo != 2 && promo != 3 && promo != 4){
                    printf("\nVEUILLEZ CHOISIR UNE PIECE PARMIS LES 4 !\n");
                }
            }while(promo != 1 && promo != 2 && promo != 3 && promo != 4);
            promo_pion(position, promo, echequier);

        }
    }


    // on regarde si le move choisi est une capture ou non
    if (moves[rep] != VIDE)
    {
        if (get_color(moves[rep], echequier) == couleur)
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

// regarde les moves de toutes les pieces ennemies: si une des pieces ennemies a le roi dans ses moves: echec pour ce roi
int verifier_echec(int *tab)
{
    int i, j = 0, echec = -1;
    int *moves = NULL;
    int taille_moves;

    // printf("\navant verif echec\n");
    for (i = 0; i < MAX; i++)
    {

        if ((tab[i] != VIDE) && (tab[i] != ROI) && (tab[i] != ROI + NOIR))
        {
            taille_moves = get_taille_moves(i, tab);
            moves = (int *)malloc(sizeof(int) * taille_moves);
            moves = recuperer_moves(i, taille_moves, tab);

            if (moves == NULL)
            {
                fprintf(stderr, "\nVERIFIER ECHEC: TABLEAU DE MOVES NULL\n");
                return -1;
            }

            j = 0;
            while (j < taille_moves)
            {
                if (tab[moves[j]] == ROI)
                { // si moves[j] contient roi blanc et que j est une piece noire
                    // printf("\nROI BLANC EN ECHEC\n");
                    echec = 0;
                }
                if (tab[moves[j]] == ROI + NOIR)
                { // si moves[j] contient roi noir et que j est une piece blanche
                    // printf("\nROI NOIR EN ECHEC\n");
                    echec = 1;
                }
                j++;
            }
        }
        free(moves);
        moves = NULL;
    }
    return echec;
}

//regarde le nombre de pieces pouvant bouger => retourne la couleur du camp perdant ou -1 si personne per
int echec_et_mat(int couleur)
{

    int i = 0, taille_liste;
    int *liste_pieces;

    if (couleur == 0)
    {
        taille_liste = compter_blanc();
    }
    else
    {
        taille_liste = compter_noir();
    }

    liste_pieces = (int *)malloc(sizeof(int) * taille_liste);
    if (liste_pieces == NULL)
    {
        fprintf(stderr, "\nUPDATE FEN: ECHEC MALLOC LISTE_PIECES -> ECHEC ET MAT NON  VERIFIE\n");
        return -1;
    }
    liste_pieces = liste_moves(couleur, liste_pieces, taille_liste, echequier);

    if (liste_pieces[0] == -1)
    {
        return couleur;
    }

    return -1;
}

FEN update_fen(FEN fen)
{
    int i;
    /*for (i = 0; i < 64; i++)
    {
        fen.tab[i] = echequier[i];
    }*/
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

    if (echequier[4] == ROI + NOIR)
    {
        fen.castleb = castle(4, echequier);
    }
    else
    {
        fen.castleb = NULL;
    }

    if (echequier[60] == ROI)
    {
        fen.castlew = castle(60, echequier);
    }
    else // si echequier[60] contient un roi qui n'a jamais bougé:
    {
        fen.castlew = NULL;
    }

    /*int * tabb = (int *)malloc(sizeof(int)*MAX);
    tabb = copie_echequier(tabb);*/
    fen.echec = verifier_echec(echequier); // modifie les valeurs de echec_blanc et echec_noir

    if (fen.echec == 0) // roi blanc en echec
    {
        // printf("\nappel de echec_et_mat pour roi blanc\n");
        fen.echec_et_mat = echec_et_mat(0);
    }
    if (fen.echec == 1) // roi noir en echec
    {
        // printf("\nappel de echec_et_mat pour roi noir\n");
        fen.echec_et_mat = echec_et_mat(1);
    }

    return fen;
}

//retourne 1 si il n'y a aucune pièce entre la case a et la case b, 0 sinon
int empty(int a, int b, int * plateau)
{
    for (int i = a; i < b; ++i)
    {
        if (plateau[i] != VIDE)
            return 0;
    }
    return 1;
}

// pour les fonctions castle on part du principe que le roi n'a jamais bougé !!
// retourne un tableau de 2 cases: moves[0] pour castle côté reine et moves[1] pour castle côté roi
int *castle(int position, int * plateau)
{
    int i = 0, pos_tour1 = -1, pos_tour2 = -1, nb_tour = 0; // pos_tour est initialisé a -1 pour preciser qu'on a pas encore de Tour
    int *moves = (int *)malloc(sizeof(int) * 2);
    // on cherche si on a une tour sur la ligne du roi
    if (get_color(position, plateau) == 1)
    {
        for (i = 0; i < 8; ++i)
        {
            if (plateau[i] == TOUR + NOIR && nb_tour == 0)
            {
                pos_tour1 = i;
                nb_tour += 1;
            }
            else
            {
                if (plateau[i] == TOUR + NOIR && nb_tour > 0)
                {
                    pos_tour2 = i;
                    nb_tour += 1;
                }
            }
        }
    }
    else
    {
        for (i = 56; i < 64; ++i)
        {
            if (plateau[i] == TOUR && nb_tour == 0)
            {
                pos_tour1 = i;
                nb_tour += 1;
            }
            else
            {
                if (plateau[i] == TOUR && nb_tour > 0)
                {
                    pos_tour2 = i;
                    nb_tour += 1;
                }
            }
        }
    }

    if (nb_tour == 0)
        return 0; // pas de tour sur la premiere ligne, donc pas de CASTLE, on sort de la fonction

    if (nb_tour == 1) // on a qu'une tour
    {
        if (pos_tour1 < position) // tour placé avant le roi
        {
            // si coté reine
            if (empty(pos_tour1 + 1, position, plateau) == 0)
            {
                moves[0] = -1; // pas de castle coté reine car pas vide
            }
            else
            {
                moves[0] = 1;
            }
        }
        else
        {
            // si coté roi
            if (empty(position + 1, pos_tour1, plateau) == 0)
            {
                moves[1] = -1; // pas de castle coté roi car pas vide
            }
            else
            {
                moves[1] = 1;
            }
        }
    }
    else // on a 2 tours
    {
        if (empty(pos_tour1 + 1, position, plateau) == 0)
        {
            moves[0] = -1; // pas de castle coté reine car pas vide
        }
        else
        {
            moves[0] = 1;
        }
        if (empty(position + 1, pos_tour2, plateau) == 0)
        {
            moves[1] = -1; // pas de castle coté roi car pas vide
        }
        else
        {
            moves[1] = 1;
        }
    }

    return moves;
}

// fonction appelée par main si on bouge un roi pour la premiere fois
void bouger_tour_castle(int position, int move)
{
    if (move == 62) // 62 correspond au castle côté roi blanc -> on bouge la tour droite
    {
        echequier[61] = echequier[63];
        echequier[63] = VIDE;
    }
    if (move == 58) // 58 correspond au castle côté reine blanche -> on bouge la tour gauche
    {
        echequier[59] = echequier[56];
        echequier[56] = VIDE;
    }
    if (move == 6) //6 = castle côté roi noir
    {
        echequier[5] = echequier[7];
        echequier[7] = VIDE;
    }
    if (move == 2) //2 = castle côté reine noire
    {
        echequier[3] = echequier[0];
        echequier[0] = VIDE;
    }
}


void promo_pion(int position, int new_piece, int *tab){
    switch(new_piece){
        case 1:
            printf("changement de PION en DAME\n");
            break;
        case 2:
            printf("changement de PION en TOUR\n");
            break;
        case 3:
            printf("changement de PION en FOU\n");
            break; 
        case 4:
            printf("changement de PION en CAVALIER\n");
            break;
        default:
            printf("ERREUR! promotion impossible : selectionnez une dame, une tour, un fou ou un cavalier !\n");
            return;
    }
    if(get_color(position, echequier) == 1){ //le pion est noir
        switch(new_piece){
            case 1: 
                tab[position] = REINE + NOIR;
                break; 
            case 2:
                tab[position] = TOUR + NOIR;
                break;
            case 3:
                tab[position] = FOU + NOIR;
                break;
            case 4:
                tab[position] = CAVALIER + NOIR;
                break;
            default:
                printf("erreur tableau\n");
                return;
        }
    }else if(get_color(position, echequier) == 0){ //le pion est blanc
        switch(new_piece){
            case 1: 
                tab[position] = REINE;
                break; 
            case 2:
                tab[position] = TOUR;
                break;
            case 3:
                tab[position] = FOU;
                break;
            case 4:
                tab[position] = CAVALIER;
                break;
            default: 
                printf("erreur tableau");
                return;
        }
    }
    printf("\nPROMOTION EFFECTUÉE !\n");
}