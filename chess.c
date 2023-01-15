#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

liste *creation_maillon(int_fast8_t n)
{
    liste *element = (liste *)malloc(sizeof(liste));
    element->valeur = n;
    element->next = NULL;
    return element;
}

// 1 si vide, 0 sinon
int_fast8_t liste_vide(liste *l)
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
int_fast8_t recherche(liste *l, int_fast8_t e)
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
liste *suppression_valeur(liste *l, int_fast8_t m)
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
int_fast8_t recuperer_valeur(liste *l, int_fast8_t n)
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
int_fast8_t get_pos(int_fast8_t ligne, int_fast8_t colonne)
{
    int_fast8_t i = ligne * 8;
    return (i + colonne);
}

int_fast8_t get_ligne(int_fast8_t position)
{
    return (position / 8);
}

int_fast8_t get_colonne(int_fast8_t position)
{
    return (position - (get_ligne(position) * 8));
}

int_fast8_t compter_pieces(int_fast8_t couleur, uint_fast8_t *plateau)
{
    int_fast8_t c = 0;
    int_fast8_t i = 0;
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

int_fast8_t get_color(uint_fast8_t piece)
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
int_fast8_t vide(int_fast8_t a, int_fast8_t b, uint_fast8_t *plateau)
{
    for (int_fast8_t i = a; i < b; ++i)
    {
        if (plateau[i] != VIDE)
            return 0;
    }
    return 1;
}

void afficher_liste_pieces(int_fast8_t couleur, uint_fast8_t *plateau)
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
    int_fast8_t c = 0;
    printf("     | ~0~ | ~1~ | ~2~ | ~3~ | ~4~ | ~5~ | ~6~ | ~7~ |\n");
    printf("+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
    for (int_fast8_t i = 0; i < TAILLE_ECHEQUIER; ++i)
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

    int_fast8_t c = 0;
    fprintf(fp, "     | ~0~ | ~1~ | ~2~ | ~3~ | ~4~ | ~5~ | ~6~ | ~7~ |\n");
    fprintf(fp, "+----+-----+-----+-----+-----+-----+-----+-----+-----+\n");
    for (int_fast8_t i = 0; i < TAILLE_ECHEQUIER; ++i)
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
    int_fast8_t c = 0;
    printf("    0  1  2  3  4  5  6  7  \n\n");
    for (int_fast8_t i = 0; i < TAILLE_ECHEQUIER; ++i)
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

void init_return_minimax()
{
    return_minimax = (best_move *)malloc(sizeof(return_minimax));
    return_minimax->move = 0;
    return_minimax->piece = 0;
    return_minimax->score = 0;
}

nouvelle_partie select_mode()
{
    nouvelle_partie partie;
    int rep = -1;
    int joueur_blanc = -1;
    int profondeur_blanc = -1;
    int joueur_noir = -1;
    int profondeur_noir = -1;
    int debug = -1;

    printf("\n\t***JEU D'ECHECS***\n");
    while (joueur_blanc != 0 && joueur_blanc != 1)
    {
        printf("\nJoueur Blanc: (0:Humain, 1:IA)\n");
        scanf("%d", &joueur_blanc);
        if (joueur_blanc != 0 && joueur_blanc != 1)
        {
            printf("\nReponse invalide!\n");
        }
    }

    while (joueur_noir != 0 && joueur_noir != 1)
    {
        printf("\nJoueur Noir: (0:Humain, 1:IA)\n");
        scanf("%d", &joueur_noir);
        if (joueur_noir != 0 && joueur_noir != 1)
        {
            printf("\nReponse invalide!\n");
        }
    }

    if (joueur_blanc == 1)
    {
        while (profondeur_blanc <= 0 || profondeur_blanc > 5)
        {
            printf("\nProfondeur de l'IA Blanche: (1-5, Recommande: 4)\n");
            scanf("%d", &profondeur_blanc);
            if (profondeur_blanc <= 0 || profondeur_blanc > 5)
            {
                printf("\nReponse invalide!\n");
            }
        }
    }

    if (joueur_noir == 1)
    {
        while (profondeur_noir <= 0 || profondeur_noir > 5)
        {
            printf("\nProfondeur de l'IA Noire: (1-5, Recommande: 4)\n");
            scanf("%d", &profondeur_noir);
            if (profondeur_noir <= 0 || profondeur_noir > 5)
            {
                printf("\nReponse invalide!\n");
            }
        }
    }

    while (debug < 0 || debug > 2)
    {
        printf("\nAfficher les infos de debug? (0:Non, 1:Oui, 2:Seulement a la fin)\n");
        scanf("%d", &debug);
        if (debug < 0 || debug > 2)
        {
            printf("\nReponse invalide!\n");
        }
    }

    partie.joueur_blanc = joueur_blanc;
    partie.joueur_noir = joueur_noir;
    partie.profondeur_blanc = profondeur_blanc;
    partie.profondeur_noir = profondeur_noir;
    partie.debug = debug;

    return partie;
}

// remplit l'echequier et initialise les structures
void initialiser_jeu()
{
    int_fast8_t i;
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
    initialiser_fen();
    init_return_minimax();
    init_hashkeys();
    hashtable = init_hashtable(hashtable);
}

void initialiser_fen()
{
    fen = (FEN *)malloc(sizeof(FEN));
    fen->tour = BLANC;
    fen->nb_pcs_w = 16;
    fen->nb_pcs_b = 16;
    fen->nb_tours = 0;
    fen->half_move = 0;
    fen->full_move = 0;
    fen->echec = -1;
    fen->echec_et_mat = -1;
    fen->capture = 0;
}

// affiche l'initiale d'une piece, utilisée pour affichage_echequier
char print_piece(uint_fast8_t position)
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

void print_name(uint_fast8_t piece)
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

void print_color(uint_fast8_t piece)
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
int select_piece(int_fast8_t tour, uint_fast8_t *plateau)
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
        fen->tour = NOIR;
        fen->full_move++;
    }
    else if (fen->tour == NOIR)
    {
        fen->tour = BLANC;
    }
    else
    {
        printf("\nERREUR, fen->tour = %d\n", fen->tour);
        fen->tour = -1;
    }

    fen->nb_pcs_b = compter_pieces(NOIR, echequier);
    fen->nb_pcs_w = compter_pieces(BLANC, echequier);
    fen->nb_tours++;

    fen->echec = verifier_echec(echequier);
    if (fen->echec == BLANC)
    {
        fen->echec_et_mat = echec_et_mat(BLANC, echequier);
    }
    else if (fen->echec == NOIR)
    {
        fen->echec_et_mat = echec_et_mat(NOIR, echequier);
    }

    fen->capture = 0;
    fen->endgame = check_endgame(echequier);
}

// retourne 1 si on est en end game, 0 sinon
// vérifie que les 2 côtés n'aient pas de reine ou que ceux qui en ont n'aient qu'une piece mineure max
int_fast8_t check_endgame(uint_fast8_t *plateau)
{
    int_fast8_t wq = 0;  // reine blanche
    int_fast8_t bq = 0;  // reine noire
    int_fast8_t wpieces; // pieces importantes blanches
    int_fast8_t bpieces; // pieces importantes noires
    int_fast8_t nb_piecesw;
    int_fast8_t nb_piecesb;
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
uint_fast8_t *copie_echequier(uint_fast8_t *plateau, uint_fast8_t *tab)
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

liste *get_legal_pion_blanc(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
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

liste *get_legal_pion_noir(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
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

liste *get_legal_cavalier(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
{
    int_fast8_t ligne = get_ligne(position);
    int_fast8_t colonne = get_colonne(position);
    int_fast8_t couleur = get_color(plateau[position]);

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

liste *get_legal_tour(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
{
    int_fast8_t ligne = get_ligne(position);
    int_fast8_t colonne = get_colonne(position);
    int_fast8_t couleur = get_color(plateau[position]);
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

liste *get_legal_fou(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
{
    int_fast8_t ligne = get_ligne(position);
    int_fast8_t colonne = get_colonne(position);
    int i;
    int_fast8_t couleur = get_color(plateau[position]);

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

liste *get_legal_roi(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
{
    int_fast8_t ligne = get_ligne(position);
    int_fast8_t colonne = get_colonne(position);
    int_fast8_t couleur = get_color(plateau[position]);

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

liste *get_legal_reine(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
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
liste *get_legal_any(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
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
liste *retirer_echec(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
{
    int_fast8_t couleur = get_color(plateau[position]);
    int_fast8_t couleur_ennemie = get_couleur_ennemie(couleur);
    int i = 0;
    liste *tmp = moves;
    liste *tmptmp;
    uint_fast8_t *echequier_tmp = (uint_fast8_t *)malloc(sizeof(uint_fast8_t) * TAILLE_ECHEQUIER);
    while (tmp != NULL)
    {
        tmptmp = tmp->next; // comme on va supprimer un élément, pas sur de ce qu'il va se passer a tmp->next donc on le save avant
        // on effectue le move
        memcpy(echequier_tmp, plateau, TAILLE_ECHEQUIER * sizeof(uint_fast8_t));
        effectuer_move(position, tmp->valeur, echequier_tmp);

        // on supprime le move de la liste si il donne lieu a un echec
        if (verifier_echec_fast(couleur, echequier_tmp) == couleur)
        {
            // printf("\nECHEC, SUPPRESSION DU MOVE %d\n", tmp->valeur);
            moves = suppression_valeur(moves, tmp->valeur);
        }

        tmp = tmptmp;
    }
    free(echequier_tmp);
    echequier_tmp = NULL;
    return moves;
}

liste *get_legal_all(int_fast8_t position, liste *moves, uint_fast8_t *plateau)
{
    moves = get_legal_any(position, moves, plateau);
    moves = retirer_echec(position, moves, plateau);

    return moves;
}

// parcours les moves de toutes les pieces et retourne la couleur du joueur en échec, -1 si aucun
int_fast8_t verifier_echec(uint_fast8_t *plateau)
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
    return echec;
}

int_fast8_t verifier_echec_fast(int_fast8_t couleur, uint_fast8_t *plateau)
{
    liste *moves = (liste *)malloc(sizeof(liste));
    moves = NULL;
    liste *tmp, *t;
    int_fast8_t i, j = 0, nb_pieces, echec = -1;

    if (couleur == BLANC) // si couleur == blanc, on cherche dans les pieces noires
    {
        nb_pieces = compter_pieces(NOIR, plateau);    // on recupere le nombre de pieces de l'ennemi
        i = 0;                                        // i = 0 car on regarde les moves des noirs, qui commencent a echequier[0]
        while (j < nb_pieces && i < TAILLE_ECHEQUIER) // tant qu'il y a des pieces a analyser (-1 car on skip les rois)
        {
            if (get_color(plateau[i]) == NOIR) // si la piece est noire
            {
                j++;                                      // on est sur une piece noire (non roi)
                moves = get_legal_any(i, moves, plateau); // on récupère ses moves
                tmp = moves;
                while (tmp != NULL) // pour chaque move de la piece
                {
                    if (plateau[tmp->valeur] == ROI || plateau[tmp->valeur] == ROI + PIECE_SPECIAL) // si le move contient un roi on peut s'arreter
                    {
                        while (moves != NULL)
                        {
                            t = moves->next;
                            free(moves);
                            moves = t;
                        }
                        return BLANC;
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
            i++;
        }
    }
    else
    {
        nb_pieces = compter_pieces(BLANC, plateau);
        i = TAILLE_ECHEQUIER - 1; // car les blancs commencent en bas
        while (j < nb_pieces && i >= 0)
        {
            if (get_color(plateau[i]) == BLANC)
            {
                j++; // on est sur une piece blanche(non roi)
                moves = get_legal_any(i, moves, plateau);
                tmp = moves;
                while (tmp != NULL)
                {
                    if (plateau[tmp->valeur] == ROI + PIECE_NOIRE || plateau[tmp->valeur] == ROI + PIECE_NOIRE + PIECE_SPECIAL)
                    {
                        while (moves != NULL)
                        {
                            t = moves->next;
                            free(moves);
                            moves = t;
                        }
                        return NOIR;
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
            i--;
        }
    }
    return echec;
}

// liste des pieces de couleur qui peuvent bouger
liste *liste_moves(int_fast8_t couleur, liste *liste_pieces, uint_fast8_t *plateau)
{
    int_fast8_t i, j = 0;
    liste *moves = (liste *)malloc(sizeof(liste));
    moves = NULL;
    liste_pieces = NULL;

    int_fast8_t nb_pieces = compter_pieces(couleur, plateau); // nombre de pieces sur l'echequier

    while (i < TAILLE_ECHEQUIER && j < nb_pieces)
    {
        // on regarde les moves dispo pour chaque piece alliée qu'on trouve
        if (get_color(plateau[i]) == couleur)
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

void promotion_user(int_fast8_t position, int_fast8_t piece, uint_fast8_t *plateau)
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
int_fast8_t echec_et_mat(int_fast8_t couleur, uint_fast8_t *plateau)
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
int_fast8_t choisir_move(int_fast8_t position, uint_fast8_t *plateau)
{
    liste *moves = (liste *)malloc(sizeof(liste));
    moves = NULL;
    moves = get_legal_all(position, moves, plateau); // get_legal_any car si on appelle bouger sur une piece, c'est quelle peut bouger
    liste *tmp = moves;
    liste *tmptmp = moves;
    int_fast8_t i = 0;
    int j = 0;
    int_fast8_t rep;

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
        int_fast8_t move2 = recuperer_valeur(tmp, rep);
        if (plateau[move2] >= 0 && plateau[move2] <= 7 && plateau[move2] >= 56 && plateau[move2] <= 63)
        {
            int_fast8_t promo;
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

void promotion_ia(int_fast8_t position, uint_fast8_t nouvelle_piece, uint_fast8_t *plateau)
{
    printf("\nappel promotion\n");
    plateau[position] = nouvelle_piece;
}

// déplace la piece dans plateau[position_piece] à la case position_move, et castle/promote si besoin
void effectuer_move(int_fast8_t position_piece, int_fast8_t position_move, uint_fast8_t *plateau)
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
        uint_fast8_t nouvelle_piece = REINE;
        plateau[position_move] = nouvelle_piece;
        plateau[position_piece] = VIDE;
    }
    else if (plateau[position_piece] == PION + PIECE_NOIRE && get_ligne(position_move) == 7) // pion noir ligne 7
    {
        uint_fast8_t nouvelle_piece = REINE + PIECE_NOIRE;
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
void ia_move(int_fast8_t profondeur, int_fast8_t couleur, int debug, uint_fast8_t *plateau)
{
    int alpha = INT_MIN;
    int beta = INT_MAX;
    FILE *fp = fopen("hashtable.txt", "a");
    if (fopen == NULL)
    {
        fprintf(stderr, "\nERREUR FOPEN\n");
        return;
    }

    float total = 0.0, temp;
    clock_t debut, fin;

    if (couleur == BLANC)
    {
        debut = clock();
        minimax_old(BLANC, 0, plateau, profondeur, alpha, beta);
        fin = clock();
        temp = (float)(fin - debut) / (float)CLOCKS_PER_SEC;
        total = temp;
    }
    if (couleur == NOIR)
    {
        debut = clock();
        minimax_old(NOIR, 1, plateau, profondeur, alpha, beta);
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
    if (debug == 1)
    {
        printf("\t***Temps de recherche: %f***\n", temp);
    }
    effectuer_move(return_minimax->piece, return_minimax->move, plateau);
}

void player_move(int_fast8_t couleur, uint_fast8_t *plateau)
{
    int_fast8_t move;
    int position;

    position = select_piece(couleur, plateau);
    move = choisir_move(position, plateau);
    effectuer_move(position, move, plateau);
}