#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// const int PvSize = 0x100000 * 2;
uint_fast8_t get_piece_index(uint_fast8_t piece)
{
    uint_fast8_t finalpiece;
    if (get_color(piece) == NOIR)
    {
        if (piece == ROI + PIECE_NOIRE + PIECE_SPECIAL)
        {
            finalpiece = 14;
        }
        else
        {
            finalpiece = (piece - PIECE_NOIRE) + 7;
        }
        return finalpiece;
    }

    else if (piece == ROI + PIECE_SPECIAL)
    {
        return 7;
    }

    return piece;
}

// initialise les clés avec des nombres aléatoires
void init_hashkeys()
{
    int index = 0;
    int index2 = 0;

    for (index = 0; index < 14; ++index)
    {
        for (index2 = 0; index2 < 64; ++index2)
        {
            PieceKeys[index][index2] = RAND_64;
        }
    }
    SideKey = RAND_64;
}

// Genere un hash unique pour chaque échequier
U64 generate_posKey(uint_fast8_t *plateau, int_fast8_t tour)
{
    int sq = 0;
    U64 FinalKey = 0;
    uint_fast8_t piece = VIDE;

    // pieces
    for (sq = 0; sq < TAILLE_ECHEQUIER; ++sq)
    {
        piece = get_piece_index(plateau[sq]);
        if (piece != VIDE)
        {
            if (piece >= PION && piece <= ROI + PIECE_NOIRE + PIECE_SPECIAL)
            {
                FinalKey ^= PieceKeys[piece][sq];
            }
            else
            {
                fprintf(stderr, "\nERREUR KEY\n");
                return 0;
            }
        }
    }

    if (tour == BLANC)
    {
        FinalKey ^= SideKey;
    }

    return FinalKey;
}

void clear_hashtable(Hash_table *table)
{
    printf("\nCLEAR\n");
    Entry *entry;
    int max_entries = MAX_TABLE_SIZE / sizeof(Entry);
    max_entries -= 2;

    int index = 0;
    while (index != MAX_TABLE_SIZE)
    {
        entry = table->entries[index];
        entry->posKey = 0ULL;
        index++;
    }
}

Hash_table *init_hashtable(Hash_table *hashtable)
{
    hashtable = (Hash_table *)malloc(sizeof(hashtable));
    if (hashtable == NULL)
    {
        fprintf(stderr, "\nHASHTABLE NULL :(\n");
        return NULL;
    }

    hashtable->nb_entries = 0;
    hashtable->entries = (Entry **)malloc(sizeof(Entry *) * MAX_TABLE_SIZE); // tableau de pointeurs vers des entrées
    if (hashtable->entries == NULL)
    {
        fprintf(stderr, "\nENTRIES NULL\n");
        return NULL;
    }
    // allocation mémoire de chaque case du tableau:
    for (int i = 0; i < MAX_TABLE_SIZE; i++)
    {
        hashtable->entries[i] = (Entry *)malloc(sizeof(Entry));
        if (hashtable->entries[i] == NULL)
        {
            fprintf(stderr, "\nEntries[%d] NULL\n", i);
            return NULL;
        }
    }
    printf("\nHashTable initialisee avec %d entries\n", hashtable->nb_entries);
    return hashtable;
}

void add_entry(Hash_table *hashtable, U64 posKey, int score)
{
    if (hashtable->nb_entries == MAX_TABLE_SIZE - 50)
    {
        return;
    }
    Entry *new_entry = (Entry *)malloc(sizeof(Entry));
    if (new_entry == NULL)
    {
        fprintf(stderr, "\nERREUR MALLOC NEWENTRY\n");
        return;
    }
    new_entry->posKey = posKey;
    new_entry->score = score;

    hashtable->entries[hashtable->nb_entries] = new_entry;
    hashtable->nb_entries++;
}

// parcours la hashtable, retourne un score si il existe une entrée avec la même clé, INT_MIN sinon
int search_table(Hash_table *hashtable, U64 posKey)
{
    for (int i = 0; i < hashtable->nb_entries; ++i) // tant que i est inférieur au nombre d'entrées dans la table
    {
        if (hashtable->entries[i]->posKey == posKey)
        {
            return hashtable->entries[i]->score;
        }
    }
    return INT_MIN; // la clé n'est pas dans la hashtable
}

void liberation_hashtable(Hash_table *hashtable)
{
    int i;
    for (i = 0; i < hashtable->nb_entries; i++)
    {

        free(hashtable->entries[i]);
    }
    free(hashtable);
}

int count_lines(FILE* file)
{
    int_fast8_t buf[65536];
    int counter = 0;
    for(;;)
    {
        size_t res = fread(buf, 1, 65536, file);
        if (ferror(file))
            return -1;

        int i;
        for(i = 0; i < res; i++)
            if (buf[i] == '\n')
                counter++;

        if (feof(file))
            break;
    }

    return counter;
}

void fill_from_file(Hash_table *hashtable)
{
    FILE *fp = fopen("aaa.txt", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERR");
        return;
    }
    int i = 0, int_buf;
    U64 llu_buf;
    int nb_lignes = 0;

    nb_lignes = 489703;

    while (i < nb_lignes)
    {
        fscanf(fp, "%llu", &llu_buf);
        fseek(fp, 3, SEEK_CUR); // on décale le pointeur de 3 pour sauter " | "
        fscanf(fp, "%d", &int_buf);
        fseek(fp, 1, SEEK_CUR); // on décale le pointeur de 1 pour sauter le "\n"
        add_entry(hashtable, llu_buf, int_buf);
        i++;
    }
}