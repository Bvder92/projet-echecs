#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

liste *creation_maillon(char n)
{
    liste *element = (liste *)malloc(sizeof(liste));
    element->valeur = n;
    element->next = NULL;
    return element;
}

// 1 si vide, 0 sinon
char liste_vide(liste *l)
{
    if (l == NULL)
        return 1;
    return 0;
}

void affichage_liste(liste *l)
{
    while (l != NULL)
    {
        printf("%d->", l->valeur);
        l = l->next;
    }
    printf("NULL");
}

// ajoute un element en tete (tete = premier element de la liste):
liste *ajout_tete(liste *l, liste *m)
{
    if (m == NULL)
    {
        return l;
    }
    if (l == NULL)
        return m;
    if (m->next == NULL)
    {
        m->next = l;
    }
    else
    {
        liste *tmp = m;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = l;
    }
    return m;
}

// supprime l'element en tete (tete = premier element de la liste):
liste *supprimer_tete(liste *l)
{
    if (l == NULL)
        return NULL;
    liste *tmp = l->next;
    free(l);
    return tmp;
}

liste *ajout_queue(liste *l, liste *m)
{
    if (l == NULL)
        return m;

    liste *tmp = l;
    while (tmp->next != NULL)
    {
        tmp = tmp->next;
    }
    tmp->next = m;
    return l;
}

liste *suppression_queue(liste *l)
{
    if (l == NULL || l->next == NULL)
        return NULL;

    liste *tmp = l;
    while (tmp->next->next != NULL)
    {
        tmp = tmp->next;
    }
    free(tmp->next);
    tmp->next = NULL;
    return l;
}

// retourne 1 si la valeur e est présente dans la liste, 0 sinon
char recherche(liste *l, char e)
{
    if (l == NULL)
        return 0;
    if (l->valeur == e)
        return 1;

    while (l->next != NULL && l->valeur != e)
    {
        l = l->next;
    }
    if (l->valeur == e)
        return 1;
    return 0;
}

// supprime la valeur m de la liste
liste *suppression_valeur(liste *l, char m)
{
    if (l == NULL)
        return NULL;

    if (l->valeur == m)
    {
        liste *tmp = l->next;
        free(l);
        return tmp;
    }

    l->next = suppression_valeur(l->next, m);
    return l;
}

// recherche et retourne la valeur du nieme maillon de la liste
char recuperer_valeur(liste *l, char n)
{
    for (int i = 0; i < n; ++i)
    {
        if (l->next != NULL)
        {
            l = l->next;
        }
        else
        {
            fprintf(stderr, "\nErreur recuperer_valeur\n");
            return 1;
        }
    }
    return l->valeur;
}

void liberation(liste *l)
{
    liste *tmp = l;
    while (l != NULL)
    {
        tmp = l->next;
        free(l);
        l = tmp;
    }
}

/* ************************
Fonctions echecs:
************************* */

// retourne une position à partir d'une ligne et colonne:
char get_pos(char ligne, char colonne)
{
    char i = ligne * 8;
    return (i + colonne);
}

char get_ligne(char position)
{
    return (position / 8);
}

char get_colonne(char position)
{
    return (position - (get_ligne(position) * 8));
}

char compter_pieces(char couleur, unsigned char *plateau)
{
    char c = 0;
    char i = 0;
    while (i < TAILLE_ECHEQUIER && c < 16)
    {
        if (get_color(plateau[i]) == couleur)
        {
            c++;
        }
        i++;
    }
    return c;
}

char get_color(unsigned char piece)
{
    if (piece > 128)
    {
        return NOIR;
    }
    else if (piece != VIDE)
    {
        return BLANC;
    }
    else
    {
        return VIDE;
    }
}

// retourne 1 si toutes les cases entre a et b sont vides
char vide(char a, char b, unsigned char *plateau)
{
    for (char i = a; i < b; ++i)
    {
        if (plateau[i] != VIDE)
            return 0;
    }
    return 1;
}

void afficher_liste_pieces(char couleur, unsigned char *plateau)
{
    int i;
    int piece = ROI;
    if (couleur == BLANC)
    {
        printf("\nPieces Blanches: ");
        while (piece >= PION)
        {
            for (i = 0; i < TAILLE_ECHEQUIER; i++)
            {
                if (plateau[i] == piece || plateau[i] == piece + PIECE_SPECIAL)
                {
                    print_name(plateau[i]);
                    printf(", ");
                }
            }
            piece--;
        }
    }
    else
    {
        piece = ROI + PIECE_NOIRE;
        printf("\nPieces Noires: ");
        while (piece >= PION + PIECE_NOIRE)
        {
            for (i = 0; i < TAILLE_ECHEQUIER; i++)
            {
                if (plateau[i] == piece || plateau[i] == piece + PIECE_SPECIAL)
                {
                    print_name(plateau[i]);
                    printf(", ");
                }
            }
            piece--;
        }
    }
}

// FONCTIONS DE BASE DU JEU:

void affichage_echequier()
{
    char c = 0;
    printf("     | ~0~ | ~1~ | ~2~ | ~3~ | ~4~ | ~5~ | ~6~ | ~7~ |\n");
    printf("+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
    for (char i = 0; i < TAILLE_ECHEQUIER; ++i)
    {
        if (i % 8 == 0)
        {
            printf("|~%d~ | ", i / 8);
        }
        printf(" %c  | ", print_piece(echequier[i]));
        c++;
        if (c == 8)
        {
            printf("\n+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
            c = 0;
        }
    }
}

int affichage_echequier_fichier()
{
    FILE *fp = fopen("partie.txt", "a");
    if (fp == NULL)
    {
        fprintf(stderr, "ERREUR OUVERTURE FICHIER");
        return 0;
    }

    char c = 0;
    fprintf(fp, "     | ~0~ | ~1~ | ~2~ | ~3~ | ~4~ | ~5~ | ~6~ | ~7~ |\n");
    fprintf(fp, "+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
    for (char i = 0; i < TAILLE_ECHEQUIER; ++i)
    {
        if (i % 8 == 0)
        {
            fprintf(fp, "|~%d~ | ", i / 8);
        }
        fprintf(fp, " %c  | ", print_piece(echequier[i]));
        c++;
        if (c == 8)
        {
            fprintf(fp, "\n+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
            c = 0;
        }
    }
    fprintf(fp, "\n\n");

    fclose(fp);
    return 1;
}

void affichage_echequier_alt()
{
    char c = 0;
    printf("    0  1  2  3  4  5  6  7  \n\n");
    for (char i = 0; i < TAILLE_ECHEQUIER; ++i)
    {
        if (i % 8 == 0)
        {
            printf(" %d ", i / 8);
        }
        printf(" %c ", print_piece(echequier[i]));
        c++;
        if (c == 8)
        {
            printf("\n");
            c = 0;
        }
    }
}

void initialiser_jeu()
{
    char i;
    echequier[0] = TOUR + PIECE_NOIRE;
    echequier[1] = CAVALIER + PIECE_NOIRE;
    echequier[2] = FOU + PIECE_NOIRE;
    echequier[3] = REINE + PIECE_NOIRE;
    echequier[4] = ROI + PIECE_NOIRE;
    echequier[5] = FOU + PIECE_NOIRE;
    echequier[6] = CAVALIER + PIECE_NOIRE;
    echequier[7] = TOUR + PIECE_NOIRE;
    for (i = 8; i < 16; i++)
    {
        echequier[i] = PION + PIECE_NOIRE;
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

void initialiser_fen()
{
    fen = (FEN *)malloc(sizeof(FEN));
    fen->tour = BLANC;
    fen->half_move = 0;
    fen->full_move = 0;
    fen->echec = -1;
    fen->echec_et_mat = -1;
    fen->capture = 0;
}

/*Plateau * init_plateau()
{
    Plateau * nouveau = (Plateau *)malloc(sizeof(Plateau));

    nouveau->plateau = (unsigned char*)malloc(sizeof(unsigned char)* TAILLE_ECHEQUIER);
    nouveau->plateau = copie_echequier(echequier, nouveau->plateau);
    nouveau->hash = GeneratePosKey(nouveau->plateau, BLANC);

    nouveau->Pvtable->numEntries = 1;

    return nouveau;
}

Plateau * update_plateau(Plateau *P)
{
    for(int i = 0; i<TAILLE_ECHEQUIER; ++i)
    {
        P->plateau[i] = echequier[i];
    }
    P->hash = GeneratePosKey(P->plateau, fen.tour);

    return P;
}*/

// affiche l'initiale d'une piece, utilisée pour affichage_echequier
char print_piece(unsigned char position)
{
    char c;
    switch (position)
    {
    case VIDE:
        c = '.';
        break;
    case PION:
    case PION + PIECE_SPECIAL:
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
    case ROI + PIECE_SPECIAL:
        c = 'k';
        break;
    case PION + PIECE_NOIRE:
    case PION + PIECE_NOIRE + PIECE_SPECIAL:
        c = 'P';
        break;
    case CAVALIER + PIECE_NOIRE:
        c = 'N';
        break;
    case FOU + PIECE_NOIRE:
        c = 'B';
        break;
    case TOUR + PIECE_NOIRE:
        c = 'R';
        break;
    case REINE + PIECE_NOIRE:
        c = 'Q';
        break;
    case ROI + PIECE_NOIRE:
        c = 'K';
        break;
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
        c = 'K';
        break;
    default:
        c = '?';
        break;
    }
    return c;
}

void print_name(unsigned char piece)
{
    switch (piece)
    {
    case VIDE:
        printf("Vide");
        break;
    case PION:
    case PION + PIECE_SPECIAL:
    case PION + PIECE_NOIRE + PIECE_SPECIAL:
    case PION + PIECE_NOIRE:
        printf("Pion");
        break;
    case CAVALIER:
    case CAVALIER + PIECE_NOIRE:
        printf("Cavalier");
        break;
    case FOU:
    case FOU + PIECE_NOIRE:
        printf("Fou");
        break;
    case TOUR:
    case TOUR + PIECE_NOIRE:
        printf("Tour");
        break;
    case REINE:
    case REINE + PIECE_NOIRE:
        printf("Reine");
        break;
    case ROI:
    case ROI + PIECE_SPECIAL:
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
    case ROI + PIECE_NOIRE:
        printf("Roi");
        break;
    default:
        printf("ERREUR");
        break;
    }
}

void print_color(unsigned char piece)
{
    if (piece > 128)
    {
        printf(" Noir");
    }
    else if (piece == VIDE)
    {
        printf("");
    }
    else
    {
        printf(" Blanc");
    }
}

// retourne la position de la piece choisie par l'utilisateur
int select_piece(char tour, unsigned char *plateau)
{
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;

    int colonne, ligne, tmp, position;
    do
    {
        tmp = 0;
        if (tour == NOIR)
        {
            printf("\nTour des Noirs\n");
            liste_pieces = liste_moves(NOIR, liste_pieces, plateau);
            printf("\nSelectionner une piece Noire:\n\tColonne: ");
        }
        else
        {
            printf("\nTour des Blancs:\n");
            liste_pieces = liste_moves(BLANC, liste_pieces, plateau);
            printf("\nSelectionner une piece Blanche:\n\tColonne: ");
        }
        scanf("%d", &colonne);
        printf("\n\tLigne: ");
        scanf("%d", &ligne);

        position = get_pos(ligne, colonne);
        if (recherche(liste_pieces, position) == 0) // recherche(liste_pieces, position)
        {
            printf("\nCette piece ne peut pas bouger ! pos = %d", position);
        }
        else
        {
            tmp = 1;
        }
    } while (tmp == 0);

    liberation(liste_pieces);

    return position;
}

void update_fen(FEN *fen)
{
    fen->half_move++;

    if (fen->capture == 1)
    {
        fen->half_move = 0;
    }

    if (fen->tour == BLANC)
    {
        // printf("fen->tour devient NOIR\n");
        fen->tour = NOIR;
        fen->full_move++;
    }
    else if (fen->tour == NOIR)
    {
        // printf("fen->tour devient BLANC\n");
        fen->tour = BLANC;
    }

    else
    {
        printf("\nERREUR, fen->tour = %d\n", fen->tour);
        fen->tour = -1;
    }

    fen->echec = verifier_echec(echequier);
    if (fen->echec == BLANC)
    {
        fen->echec_et_mat = echec_et_mat(BLANC, echequier);
    }
    else if (fen->echec == NOIR)
    {
        fen->echec_et_mat = echec_et_mat(NOIR, echequier);
    }
    /*int echec = verifier_echec_couleur(fen->tour, echequier);
    if (echec == NOIR)
    {
        fen->echec = NOIR;
    }
    else if (echec == BLANC)
    {
        fen->echec = BLANC;
    }

    if (fen->echec != -1)
    {
        fen->echec_et_mat = echec_et_mat(fen->echec, echequier);
    }*/

    fen->capture = 0;

    fen->endgame = check_endgame(echequier);
}

// retourne 1 si on est en end game, 0 sinon
// vérifie que les 2 côtés n'aient pas de reine ou que ceux qui en ont n'aient qu'une piece mineure max
char check_endgame(unsigned char *plateau)
{
    char wq = 0;  // reine blanche
    char bq = 0;  // reine noire
    char wpieces; // pieces importantes blanches
    char bpieces; // pieces importantes noires
    char nb_piecesw;
    char nb_piecesb;
    int i;
    for (i = 0; i < TAILLE_ECHEQUIER; ++i)
    {
        if (plateau[i] == REINE)
        {
            wq = 1;
        }
        if (plateau[i] == REINE + PIECE_NOIRE)
        {
            bq = 1;
        }
    }

    if (wq == 0 && bq == 0) // les 2 côtés n'ont pas de reine
    {
        return 1;
    }

    if (wq == 1) // les blancs ont une reine
    {
        nb_piecesw = compter_pieces(BLANC, plateau);
        for (i = 0; i < TAILLE_ECHEQUIER; ++i)
        {
            if (plateau[i] == TOUR) // la tour n'est pas une piece mineure, on est pas en endgame
            {
                return 0;
            }
            if (plateau[i] == CAVALIER || plateau[i] == FOU)
            {
                wpieces++;
                if (wpieces > 1) // on a + d'une piece mineure, pas d'endgame
                {
                    return 0;
                }
            }
        }
    }
    if (bq == 1) // les noirs ont une reine
    {
        nb_piecesb = compter_pieces(NOIR, plateau);
        for (i = 0; i < TAILLE_ECHEQUIER; ++i)
        {
            if (plateau[i] == TOUR + PIECE_NOIRE)
            {
                return 0;
            }
            if (plateau[i] == CAVALIER + PIECE_NOIRE || plateau[i] == FOU + PIECE_NOIRE)
            {
                bpieces++;
                if (bpieces > 1)
                {
                    return 0;
                }
            }
        }
    }

    return 1; // si on a pas encore return c'est que les conditions sont respectées
}

// parametre 1 = tableau original, parametre 2 = nouveau tableau
unsigned char *copie_echequier(unsigned char *plateau, unsigned char *tab)
{
    for (int i = 0; i < TAILLE_ECHEQUIER; ++i)
    {
        tab[i] = plateau[i];
    }
    return tab;
}

int get_pos_roi(int couleur)
{
    int i = 0;
    if (couleur == NOIR)
    {
        while (i < TAILLE_ECHEQUIER)
        {
            if (echequier[i] == ROI + PIECE_NOIRE || echequier[i] == ROI + PIECE_NOIRE + PIECE_SPECIAL)
            {
                return i;
            }
            i++;
        }
    }
    else if (couleur == BLANC)
    {
        while (i < TAILLE_ECHEQUIER)
        {
            if (echequier[i] == ROI || echequier[i] == ROI + PIECE_SPECIAL)
            {
                return i;
            }
            i++;
        }
    }
    else
    {
        return -1;
    }
}

/* ********************************
RECUPERATION DE MOVES:
**********************************/

liste *get_legal_pion_blanc(char position, liste *moves, unsigned char *plateau)
{

    // si la case devant le pion est vide, on peut avancer:
    if (plateau[position - 8] == VIDE)
    {
        moves = ajout_tete(moves, creation_maillon(position - 8));
    }

    // si les deux cases devant le pion sont vides et que le pion n'a jamais bougé on peut double avancer:
    if ((plateau[position - 8] == VIDE) && (plateau[position - 16] == VIDE))
    {
        if (get_ligne(position) == 6)
        {
            moves = ajout_tete(moves, creation_maillon(position - 16));
        }
    }

    // si la case en haut à droite/gauche contient un pion noir on peut y aller (capture):
    if ((get_colonne(position) != 7) && (get_color(plateau[position - 7]) == NOIR))
    {
        moves = ajout_tete(moves, creation_maillon(position - 7));
    }

    if ((get_colonne(position) != 0) && (get_color(plateau[position - 9]) == NOIR))
    {
        moves = ajout_tete(moves, creation_maillon(position - 9));
    }

    return moves;
}

liste *get_legal_pion_noir(char position, liste *moves, unsigned char *plateau)
{

    if (plateau[position + 8] == VIDE)
        moves = ajout_tete(moves, creation_maillon(position + 8));

    if ((plateau[position + 8] == VIDE) && (plateau[position + 16] == VIDE))
    {
        if (get_ligne(position) == 1)
        {
            moves = ajout_tete(moves, creation_maillon(position + 16));
        }
    }

    // si la case en bas a droite/gauche contient un pion blanc on peut y aller

    if ((get_colonne(position) != 0) && (get_color(plateau[position + 7]) == BLANC))
    {
        moves = ajout_tete(moves, creation_maillon(position + 7));
    }

    if ((get_colonne(position) != 7) && (get_color(plateau[position + 9]) == BLANC))
    {
        moves = ajout_tete(moves, creation_maillon(position + 9));
    }

    return moves;
}

liste *get_legal_cavalier(char position, liste *moves, unsigned char *plateau)
{
    char ligne = get_ligne(position);
    char colonne = get_colonne(position);
    char couleur = get_color(plateau[position]);

    if (position - 17 >= 0 && colonne >= 1 && ligne >= 2 && get_color(plateau[position - 17]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 17));
    }
    if (position - 15 >= 0 && colonne <= 6 && ligne >= 2 && get_color(plateau[position - 15]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 15));
    }
    if (position - 10 >= 0 && colonne >= 2 && ligne >= 1 && get_color(plateau[position - 10]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 10));
    }
    if (position - 6 >= 0 && colonne <= 5 && ligne >= 1 && get_color(plateau[position - 6]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 6));
    }

    if (position + 6 <= 63 && colonne >= 2 && ligne <= 6 && get_color(plateau[position + 6]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 6));
    }
    if (position + 10 <= 63 && colonne <= 5 && ligne <= 6 && get_color(plateau[position + 10]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 10));
    }
    if (position + 15 <= 63 && colonne >= 1 && ligne <= 5 && get_color(plateau[position + 15]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 15));
    }
    if (position + 17 <= 63 && colonne <= 6 && ligne <= 5 && get_color(plateau[position + 17]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 17));
    }

    return moves;
}

liste *get_legal_tour(char position, liste *moves, unsigned char *plateau)
{
    char ligne = get_ligne(position);
    char colonne = get_colonne(position);
    char couleur = get_color(plateau[position]);
    int i, j;

    for (i = position - 8; i >= 0; i = i - 8)
    {
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        j++;
        if (plateau[i] != VIDE)
        {
            break;
        }
    }

    for (i = position + 8; i < 64; i = i + 8)
    {
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        j++;
        if (plateau[i] != VIDE)
        {
            break;
        }
    }

    i = position + 1; // on initialise i à position+1 et on va l'incrémenter tant qu'il ne change pas de ligne
    while ((get_ligne(i) == ligne) && (i < 64) && (i >= 0))
    { // ligne horizontale vers la droite: on ajoute 1 à i tant qu'il ne change pas de ligne
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        j++;
        if (plateau[i] != VIDE)
        {
            break;
        }
        i++;
    }

    i = position - 1;
    while ((get_ligne(i) == ligne) && (i < 64) && (i >= 0))
    { // ligne horizontale vers la gauche: on retire 1 à i tant qu'il ne change pas de ligne
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        j++;
        if (plateau[i] != VIDE)
        {
            break;
        }
        i--;
    }
    return moves;
}

liste *get_legal_fou(char position, liste *moves, unsigned char *plateau)
{
    char ligne = get_ligne(position);
    char colonne = get_colonne(position);
    int i;
    char couleur = get_color(plateau[position]);

    i = position - 9;
    while ((get_colonne(i) < colonne) && (i >= 0))
    { // diagonale vers le haut gauche: on décrémente de 9 tant qu'on est dans l'échequier
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        if (plateau[i] != VIDE)
        {
            break;
        }
        i = i - 9;
    }
    i = position - 7;
    while ((get_colonne(i) > colonne) && (i >= 0))
    { // diagonale vers le haut droit: on décrémente de 7 tant qu'on est dans l'échequier
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        if (plateau[i] != VIDE)
        {
            break;
        }
        i = i - 7;
    }
    i = position + 7;
    while ((get_colonne(i) < colonne) && (i < 64))
    { // diagonale vers le bas droit: on incrémente de 9 tant qu'on est dans l'échequier
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        if (plateau[i] != VIDE)
        {
            break;
        }
        i = i + 7;
    }
    i = position + 9;
    while ((get_colonne(i) > colonne) && (i < 64))
    { // diagonale vers le bas gauche: on incrémente de 7 tant qu'on est dans l'échequier
        if (get_color(plateau[i]) != couleur)
        {
            moves = ajout_tete(moves, creation_maillon(i));
        }
        if (plateau[i] != VIDE)
        {
            break;
        }
        i = i + 9;
    }
    return moves;
}

liste *get_legal_roi(char position, liste *moves, unsigned char *plateau)
{
    char ligne = get_ligne(position);
    char colonne = get_colonne(position);
    char couleur = get_color(plateau[position]);

    if (colonne != 0 && ligne != 0 && get_color(plateau[position - 9]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 9));
    }
    if (ligne != 0 && get_color(plateau[position - 8]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 8));
    }
    if (colonne != 7 && ligne != 0 && get_color(plateau[position - 7]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 7));
    }
    if (colonne != 0 && get_color(plateau[position - 1]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position - 1));
    }
    if (colonne != 7 && get_color(plateau[position + 1]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 1));
    }
    if (colonne != 0 && ligne != 7 && get_color(plateau[position + 7]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 7));
    }
    if (ligne != 7 && get_color(plateau[position + 8]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 8));
    }
    if (colonne != 7 && ligne != 7 && get_color(plateau[position + 9]) != couleur)
    {
        moves = ajout_tete(moves, creation_maillon(position + 9));
    }

    if (plateau[position] == ROI && plateau[61] == VIDE && plateau[62] == VIDE) // castle blanc coté roi
    {
        moves = ajout_tete(moves, creation_maillon(position + 2));
    }

    if (plateau[position] == ROI && vide(57, 60, plateau) == 1) // castle blanc coté reine
    {
        moves = ajout_tete(moves, creation_maillon(position - 2));
    }

    if (plateau[position] == ROI + PIECE_NOIRE && plateau[5] == VIDE && plateau[6] == VIDE) // castle noir coté roi
    {
        moves = ajout_tete(moves, creation_maillon(position + 2));
    }

    if (plateau[position] == ROI + PIECE_NOIRE && vide(1, 4, plateau) == 1) // castle noir coté reine
    {
        moves = ajout_tete(moves, creation_maillon(position - 2));
    }

    return moves;
}

liste *get_legal_reine(char position, liste *moves, unsigned char *plateau)
{
    liste *moves_fou = (liste *)malloc(sizeof(liste));
    liste *moves_tour = (liste *)malloc(sizeof(liste));
    moves_fou = NULL;
    moves_tour = NULL;
    moves_fou = get_legal_fou(position, moves_fou, plateau);
    moves_tour = get_legal_tour(position, moves_tour, plateau);
    liste *tmp_tour = moves_tour;
    liste *tmp_fou = moves_fou;

    while (tmp_fou != NULL)
    {
        moves = ajout_tete(moves, creation_maillon(tmp_fou->valeur));
        tmp_fou = tmp_fou->next;
    }

    while (tmp_tour != NULL)
    {
        moves = ajout_tete(moves, creation_maillon(tmp_tour->valeur));
        tmp_tour = tmp_tour->next;
    }

    liberation(moves_fou);
    liberation(moves_tour);

    return moves;
}

// appelle get_legal sur la piece dans plateau[position]
liste *get_legal_any(char position, liste *moves, unsigned char *plateau)
{
    switch (plateau[position])
    {
    case PION:
    case PION + PIECE_SPECIAL:
        moves = get_legal_pion_blanc(position, moves, plateau);
        break;
    case PION + PIECE_NOIRE:
    case PION + PIECE_NOIRE + PIECE_SPECIAL:
        moves = get_legal_pion_noir(position, moves, plateau);
        break;
    case CAVALIER:
    case CAVALIER + PIECE_NOIRE:
        moves = get_legal_cavalier(position, moves, plateau);
        break;
    case FOU:
    case FOU + PIECE_NOIRE:
        moves = get_legal_fou(position, moves, plateau);
        break;
    case TOUR:
    case TOUR + PIECE_NOIRE:
        moves = get_legal_tour(position, moves, plateau);
        break;
    case REINE:
    case REINE + PIECE_NOIRE:
        moves = get_legal_reine(position, moves, plateau);
        break;
    case ROI:
    case ROI + PIECE_SPECIAL:
        moves = get_legal_roi(position, moves, plateau);
        break;
    case ROI + PIECE_NOIRE:
    case ROI + PIECE_NOIRE + PIECE_SPECIAL:
        moves = get_legal_roi(position, moves, plateau);
        break;
    default:
        fprintf(stderr, "ERREUR RECUP MOVES: PIECE INCONNUE, piece = %d, position = %d\n", plateau[position], position);
    }
    return moves;
}

// retire les moves qui vont donner lieu a un echec de la liste de moves d'une piece
liste *retirer_echec(char position, liste *moves, unsigned char *plateau)
{
    char couleur = get_color(plateau[position]);
    char couleur_ennemie = get_couleur_ennemie(couleur);
    int i = 0;
    liste *tmp = moves;
    liste *tmptmp;
    unsigned char *echequier_tmp = (unsigned char *)malloc(sizeof(unsigned char) * TAILLE_ECHEQUIER);
    while (tmp != NULL)
    {
        tmptmp = tmp->next; // comme on va supprimer un élément, pas sur de ce qu'il va se passer a tmp->next donc on le save avant
        // on effectue le move
        memcpy(echequier_tmp, plateau, TAILLE_ECHEQUIER * sizeof(unsigned char));
        echequier_tmp[tmp->valeur] = echequier_tmp[position];
        echequier_tmp[position] = VIDE;

        // on supprime le move de la liste si il donne lieu a un echec
        if (verifier_echec_fast(couleur, echequier_tmp) == couleur)
        {
            //printf("\nECHEC, SUPPRESSION DU MOVE %d\n", tmp->valeur);
            moves = suppression_valeur(moves, tmp->valeur);
        }

        tmp = tmptmp;
    }
    free(echequier_tmp);
    echequier_tmp = NULL;
    return moves;
}

liste *get_legal_all(char position, liste *moves, unsigned char *plateau)
{
    moves = get_legal_any(position, moves, plateau);
    moves = retirer_echec(position, moves, plateau);

    return moves;
}

// parcours les moves de toutes les pieces et retourne la couleur du joueur en échec, -1 si aucun
char verifier_echec(unsigned char *plateau)
{
    liste *moves = (liste *)malloc(sizeof(liste));
    moves = NULL;
    liste *tmp; // parcours
    liste *t;   // liberation
    int i, j = 0, echec = -1;
    for (i = 0; i < TAILLE_ECHEQUIER; i++)
    {
        if ((plateau[i] != VIDE) && (plateau[i] != ROI) && (plateau[i] != ROI + PIECE_NOIRE) && (plateau[i] != ROI + PIECE_SPECIAL) && (plateau[i] != ROI + PIECE_NOIRE + PIECE_SPECIAL))
        {
            moves = get_legal_any(i, moves, plateau);
            tmp = moves;
            while (tmp != NULL)
            {
                if (plateau[tmp->valeur] == ROI || plateau[tmp->valeur] == ROI + PIECE_SPECIAL)
                {
                    echec = BLANC;
                }
                if (plateau[tmp->valeur] == ROI + PIECE_NOIRE || plateau[tmp->valeur] == ROI + PIECE_NOIRE + PIECE_SPECIAL)
                {
                    echec = NOIR;
                }
                tmp = tmp->next;
            }
            while (moves != NULL)
            {
                t = moves->next;
                free(moves);
                moves = t;
            }
        }
    }
    free(moves);
    return echec;
}

char verifier_echec_fast(char couleur, unsigned char *plateau)
{    
    liste *moves = (liste *)malloc(sizeof(liste));
    moves = NULL;
    liste *tmp; // parcours
    liste *t;   // liberation
    char i = 0, j = 0, echec = -1;
    char nb_pieces = compter_pieces(couleur, plateau);

    if (couleur == BLANC) //on regarde les moves des noirs -> i commence a 0 car les noirs commencent en haut
    {
        while (j < nb_pieces - 1 && i < TAILLE_ECHEQUIER)
        {
            if (plateau[i] != ROI+PIECE_NOIRE && plateau[i] != ROI+PIECE_NOIRE+PIECE_SPECIAL && get_color(plateau[i]) == NOIR)
            {
                moves = get_legal_any(i, moves, plateau);
                tmp = moves;
                while (tmp != NULL)
                {
                    if (plateau[tmp->valeur] == ROI || plateau[tmp->valeur] == ROI + PIECE_SPECIAL)
                    {
                        //printf("\nROI BLANC DANS LES MOVES DE ");
                        echec = couleur;
                    }
                    tmp = tmp->next;
                }
                while (moves != NULL)
                {
                    t = moves->next;
                    free(moves);
                    moves = t;
                }
                ++j;
            }
            ++i;
        }
    }
    else
    {
        i = TAILLE_ECHEQUIER-1; //i commence en 63 et décrémente car les blancs commencent en bas
        while (j < nb_pieces - 1 && i >= 0)
        {
            if (plateau[i] != ROI && plateau[i] != ROI+PIECE_SPECIAL && get_color(plateau[i]) == BLANC)
            {
                moves = get_legal_any(i, moves, plateau);
                tmp = moves;
                while (tmp != NULL)
                {
                    if (plateau[tmp->valeur] == ROI+PIECE_NOIRE || plateau[tmp->valeur] == ROI+PIECE_NOIRE+PIECE_SPECIAL)
                    {
                        //printf("\nROI NOIR DANS LES MOVES DE ");
                        //print_piece(tmp->valeur);
                        //print_color(tmp->valeur);
                        //printf("\n");
                        echec = couleur;
                    }
                    tmp = tmp->next;
                }
                while (moves != NULL)
                {
                    t = moves->next;
                    free(moves);
                    moves = t;
                }
                ++j;
            }
            --i;
        }
    }
    free(moves);
    return echec;
}

// verifie si la couleur passée en argument est en echec en inspectant les moves de toutes les pieces ennemies
char verifier_echec_couleur(char couleur, unsigned char *plateau)
{
    char couleur_ennemie = get_couleur_ennemie(couleur), echec = -1;
    unsigned char const_couleur;

    if (couleur == NOIR)
    {
        const_couleur = PIECE_NOIRE;
    }
    else
    {
        const_couleur = 0;
    }
    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;
    liste *tmp_moves, *tmp_pieces;
    liste_pieces = liste_moves(couleur_ennemie, liste_pieces, plateau); // on recupere la liste des pieces ennemies pouvant bouger

    tmp_pieces = liste_pieces;
    while (tmp_pieces != NULL)
    {
        liste *moves = (liste *)malloc(sizeof(liste));
        moves = NULL;
        moves = get_legal_any(tmp_pieces->valeur, moves, plateau);
        tmp_moves = moves;

        while (tmp_moves != NULL)
        {
            if (plateau[tmp_moves->valeur] == ROI + const_couleur || plateau[tmp_moves->valeur] == ROI + const_couleur + PIECE_SPECIAL)
            {
                echec = couleur;
            }
            tmp_moves = tmp_moves->next;
        }
        tmp_pieces = tmp_pieces->next;
        liberation(moves);
    }
    liberation(liste_pieces);
    return echec;
}

// liste des pieces de couleur qui peuvent bouger
liste *liste_moves(char couleur, liste *liste_pieces, unsigned char *plateau)
{
    char i, j = 0;
    liste *moves = (liste *)malloc(sizeof(liste));
    moves = NULL;
    liste_pieces = NULL;

    char nb_pieces = compter_pieces(couleur, plateau); // nombre de pieces sur l'echequier

    while (i < TAILLE_ECHEQUIER && j < nb_pieces)
    {
        // on regarde les moves dispo pour chaque piece alliée qu'on trouve
        if ((plateau[i] != VIDE) && (get_color(plateau[i]) == couleur))
        {
            ++j;
            moves = get_legal_all(i, moves, plateau); // get_legal_all ne retourne pas les moves qui vont causer un échec

            // on regarde si il y a au moins 1 move dispo:
            if (moves != NULL)
            {
                liste_pieces = ajout_tete(liste_pieces, creation_maillon(i)); // si il y a au moins 1 move, on ajoute la piece à la liste de pieces qui peuvent bouger
            }
            liste *t = moves;
            while (moves != NULL)
            {
                t = moves->next;
                free(moves);
                moves = t;
            }
        }
        ++i;
    }

    return liste_pieces;
}

void promotion_user(char position, char piece, unsigned char *plateau)
{
    switch (piece)
    {
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
    if (get_color(position) == 1)
    { // le pion est noir
        switch (piece)
        {
        case 1:
            plateau[position] = REINE + PIECE_NOIRE;
            break;
        case 2:
            plateau[position] = TOUR + PIECE_NOIRE;
            break;
        case 3:
            plateau[position] = FOU + PIECE_NOIRE;
            break;
        case 4:
            plateau[position] = CAVALIER + PIECE_NOIRE;
            break;
        default:
            printf("erreur tableau\n");
            return;
        }
    }
    else if (get_color(position) == 0)
    { // le pion est blanc
        switch (piece)
        {
        case 1:
            plateau[position] = REINE;
            break;
        case 2:
            plateau[position] = TOUR;
            break;
        case 3:
            plateau[position] = FOU;
            break;
        case 4:
            plateau[position] = CAVALIER;
            break;
        default:
            printf("erreur tableau\n");
            return;
        }
    }
    printf("PROMOTION EFFECTUÉE !\n");
}

// retourne la couleur du perdant, ou -1
char echec_et_mat(char couleur, unsigned char *plateau)
{

    liste *liste_pieces = (liste *)malloc(sizeof(liste));
    liste_pieces = NULL;
    liste_pieces = liste_moves(couleur, liste_pieces, plateau);

    if (liste_pieces == NULL)
    {
        liste *tmp = liste_pieces;
        while (liste_pieces != NULL)
        {
            tmp = liste_pieces->next;
            free(liste_pieces);
            liste_pieces = tmp;
        }
        return couleur;
    }
    else
    {
        liste *tmp = liste_pieces;
        while (liste_pieces != NULL)
        {
            tmp = liste_pieces->next;
            free(liste_pieces);
            liste_pieces = tmp;
        }
        return -1;
    }
}

// propose tous les moves dispo a l'utilisateur et retourne le move choisi:
char choisir_move(char position, unsigned char *plateau)
{
    liste *moves = (liste *)malloc(sizeof(liste));
    moves = NULL;
    moves = get_legal_all(position, moves, plateau); // get_legal_any car si on appelle bouger sur une piece, c'est quelle peut bouger
    liste *tmp = moves;
    liste *tmptmp = moves;
    char i = 0;
    int j = 0;
    char rep;

    printf("\nPiece selectionnee: ");
    print_name(plateau[position]);
    print_color(plateau[position]);
    printf("(%d)", plateau[position]);
    printf("\n");

    // affichage des moves disponibles:
    printf("\nMoves possibles: ");
    while (tmp != NULL)
    {
        printf("%d: (%d, %d)  ", i, get_colonne(tmp->valeur), get_ligne(tmp->valeur));
        i++;
        tmp = tmp->next;
    }

    // choix de l'utilisateur, i = nombre de moves dispo + 1.
    do
    {
        printf("Choisir move: ");
        scanf("%d", &rep);
        if (rep < 0 || rep > i - 1)
        {
            printf("\nChoisir un move valide!\n");
        }
    } while (rep < 0 || rep > i - 1);

    // on a une rep comprise entre 0 et i-1 (le nombre de moves dispo)
    while (j < rep)
    {
        tmptmp = tmptmp->next;
        j++;
    }
    rep = tmptmp->valeur;

    // promotion pour l'utilisateur
    /*if (plateau[position] == PION || plateau[position] == PION + PIECE_NOIRE)
    {
        char move2 = recuperer_valeur(tmp, rep);
        if (plateau[move2] >= 0 && plateau[move2] <= 7 && plateau[move2] >= 56 && plateau[move2] <= 63)
        {
            char promo;
            printf("\nVOUS ETES EN SITUATION DE PROMOTION, VEUILLEZ SELECTIONNER UNE PIECE PARMIS = 1 : DAME, 2 : TOUR, 3 : FOU, 4 : CAVALIER\n");
            scanf("%d", &promo);
            promotion_user(position, promo, plateau);
        }
    }*/

    liste *t = moves;
    while (moves != NULL)
    {
        t = moves->next;
        free(moves);
        moves = t;
    }
    t = tmp = tmptmp = NULL;

    return rep;
}

void promotion_ia(char position, unsigned char nouvelle_piece, unsigned char *plateau)
{
    printf("\nappel promotion\n");
    plateau[position] = nouvelle_piece;
}

void effectuer_move(char position_piece, char position_move, unsigned char *plateau)
{
    if (plateau[position_piece] == ROI && position_move == 62) // castle blanc coté roi
    {
        plateau[62] = ROI + PIECE_SPECIAL;
        plateau[position_piece] = VIDE;
        plateau[61] = plateau[63];
        plateau[63] = VIDE;
    }

    else if (plateau[position_piece] == ROI && position_move == 58) // blanc coté reine
    {
        plateau[58] = ROI + PIECE_SPECIAL;
        plateau[position_piece] = VIDE;
        plateau[59] = plateau[56];
        plateau[56] = VIDE;
    }

    else if (plateau[position_piece] == ROI + PIECE_NOIRE && position_move == 6) // castle noir roi
    {
        plateau[6] = ROI + PIECE_NOIRE + PIECE_SPECIAL;
        plateau[position_piece] = VIDE;
        plateau[5] = plateau[7];
        plateau[7] = VIDE;
    }

    else if (plateau[position_piece] == ROI + PIECE_NOIRE && position_move == 2) // castle noir coté reine
    {
        plateau[2] = ROI + +PIECE_NOIRE + PIECE_SPECIAL;
        plateau[position_piece] = VIDE;
        plateau[3] = plateau[0];
        plateau[0] = VIDE;
    }

    // position move car on part du principe que le pion n'a pas encore bougé donc regarder sa position avant son move est inutile
    else if (plateau[position_piece] == PION && get_ligne(position_move) == 0) // pion blanc ligne 0
    {
        unsigned char nouvelle_piece = REINE;
        plateau[position_move] = nouvelle_piece;
        plateau[position_piece] = VIDE;
    }
    else if (plateau[position_piece] == PION + PIECE_NOIRE && get_ligne(position_move) == 7) // pion noir ligne 7
    {
        unsigned char nouvelle_piece = REINE + PIECE_NOIRE;
        plateau[position_move] = nouvelle_piece;
        plateau[position_piece] = VIDE;
    }

    else
    {
        // Ajout de la constante spéciale si on bouge un roi:
        if (plateau[position_piece] == ROI || plateau[position_piece] == ROI + PIECE_NOIRE)
        {
            plateau[position_piece] = plateau[position_piece] + PIECE_SPECIAL;
        }

        plateau[position_move] = plateau[position_piece];
        plateau[position_piece] = VIDE;
    }
}

// apppelle minimax pour savoir le meilleur move possible, puis l'effectue
void ia_move(char profondeur, char couleur, unsigned char *plateau)
{
    int alpha = INT_MIN;
    int beta = INT_MAX;

    float total = 0.0, temp;
    clock_t debut, fin;

    if (couleur == BLANC)
    {
        debut = clock();
        minimax(BLANC, 0, plateau, profondeur, alpha, beta);
        fin = clock();
        temp = (float)(fin - debut) / (float)CLOCKS_PER_SEC;
        total = temp;
    }
    if (couleur == NOIR)
    {
        debut = clock();
        minimax(NOIR, 1, plateau, profondeur, alpha, beta);
        fin = clock();
        temp = (float)(fin - debut) / (float)CLOCKS_PER_SEC;
        total = temp;
    }
    total_search += total;

    printf("\nPiece: ");
    print_name(plateau[return_minimax->piece]);
    print_color(plateau[return_minimax->piece]);
    printf("(%d,%d), ", get_colonne(return_minimax->piece), get_ligne(return_minimax->piece));
    printf("Move: ");
    print_name(plateau[return_minimax->move]);
    print_color(plateau[return_minimax->move]);
    printf("(%d,%d), ", get_colonne(return_minimax->move), get_ligne(return_minimax->move));
    printf("Score: %d\n", return_minimax->score);
    printf("\t***Temps de recherche: %f***\n", temp);
    effectuer_move(return_minimax->piece, return_minimax->move, plateau);
}

void player_move(char couleur, unsigned char *plateau)
{
    char move;
    int position;

    position = select_piece(couleur, plateau);
    move = choisir_move(position, plateau);
    effectuer_move(position, move, plateau);
}