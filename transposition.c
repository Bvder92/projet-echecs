#include "chess.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

//const int PvSize = 0x100000 * 2;
unsigned char get_piece_index(unsigned char piece)
{
    unsigned char finalpiece;
    if (get_color(piece) == NOIR)
    {
        if (piece == ROI+PIECE_NOIRE+PIECE_SPECIAL)
        {
            finalpiece = 14;
        }
        else
        {
        finalpiece = (piece - PIECE_NOIRE) + 7;
        }
        return finalpiece;
    }

    else if (piece == ROI+PIECE_SPECIAL)
    {
        return 7;
    }

    return piece;
}

//initialise les clés avec des nombres aléatoires
void InitHashKeys()
{
    int index = 0;
    int index2 = 0;

    for (index = 0; index < 14; ++index)
    {
        for (index2 = 0; index2<64; ++index2)
        {
            PieceKeys[index][index2] = RAND_64;
        }
    }
    SideKey = RAND_64;
}

//Genere un hash unique pour chaque échequier
U64 GeneratePosKey(unsigned char * plateau, char tour)
{
    int sq = 0;
    U64 FinalKey = 0;
    unsigned char piece = VIDE;

    //pieces
    for (sq = 0; sq<TAILLE_ECHEQUIER; ++sq)
    {
        piece = get_piece_index(plateau[sq]);
        if (piece != VIDE)
        {
            if (piece >= PION && piece<= ROI+PIECE_NOIRE+PIECE_SPECIAL)
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

void clear_hashtable(Hash_table * table)
{
    printf("\nCLEAR\n");
    Entry * entry;
    int max_entries = MAX_TABLE_SIZE / sizeof(Entry);
    max_entries -= 2;

    int index = 0;
    while(index != MAX_TABLE_SIZE)
    {
        entry = table->entries[index];
        entry->posKey = 0ULL;
        index++;
    }
}

void init_hashtable(Hash_table * hashtable)
{
    hashtable = (Hash_table*)malloc(sizeof(hashtable));

    //hashtable->nb_entries = 0;
    hashtable->entries = (Entry**)malloc(sizeof(Entry*)*MAX_TABLE_SIZE); //tableau de pointeurs vers des entrées

    //allocation mémoire de chaque case du tableau:
    for (int i = 0; i<MAX_TABLE_SIZE; i++) 
    {
        hashtable->entries[i] = (Entry*)malloc(sizeof(Entry));
    }
    printf("\nHashTable initialisee avec %d entries\n", hashtable->nb_entries);
}

void add_entry(Hash_table * hashtable, U64 posKey, int score)
{

    Entry *new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->posKey = posKey;
    new_entry->score = score;
    //new_entry->next = hashtable->entries[hashtable->index];

    hashtable->entries[hashtable->nb_entries] = new_entry;
    hashtable->nb_entries++;
}

//parcours la hashtable, retourne un score si il existe une entrée avec la même clé, INT_MIN sinon
int search_table(Hash_table *hashtable, U64 posKey)
{
    for (int i = 0; i<hashtable->nb_entries; ++i) //tant que i est inférieur au nombre d'entrées dans la table
    {
        if (hashtable->entries[i]->posKey = posKey)
        {
            return hashtable->entries[i]->score;
        }
    }
    return INT_MIN; //la clé n'est pas dans la hashtable
}

/*
void ClearPvTable(S_PVTABLE * table)
{
    S_PVENTRY * pvEntry;

    //on utilise un pointeur qui parcours toutes les entries de la table (pointeur++ incrémente de sizeof(élément pointé))
    for (pvEntry = table->pTable; pvEntry < table->pTable + table->numEntries; pvEntry++)
    {
        pvEntry->posKey = 0ULL;
    }
}

void InitPvTable(S_PVTABLE * table)
{
    table->numEntries = PvSize / sizeof(S_PVENTRY);
    table->numEntries -= 2;
    free(table->pTable); //libere le pointeur en cas de réutilisation

    table->pTable = (S_PVENTRY *)malloc(table->numEntries * sizeof(S_PVENTRY));
    ClearPvTable(table);
    printf("\nPvTable init complete with %d entries\n", table->numEntries);
}

void StorePvMove(Plateau *p, int move)
{
    int index = p->hash % p->Pvtable->numEntries;

    if (index < 0 || index > p->Pvtable->numEntries - 1)
    {
        fprintf(stderr, "\nERREUR STORE\n");
    }
    p->Pvtable->pTable[index].position_move = move;
    p->Pvtable->pTable[index].posKey = p->hash;

}

int SearchPvTable(Plateau *p, int move)
{
    printf("\np->hash = %llu, p->Pvtable->numEntries = %d\n", p->hash, p->Pvtable->numEntries);
    int index = p->hash % p->Pvtable->numEntries;

    if (p->Pvtable->pTable[index].posKey == p->hash)
    {
        return p->Pvtable->pTable[index].posKey;
    }
    return 0;
}
*/
