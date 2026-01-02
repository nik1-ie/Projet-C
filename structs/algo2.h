#ifndef ALGO_TRIE_H
#define ALGO_TRIE_H

#include <stddef.h>
#include "alloc.h"

typedef struct {
    char *mot;
    int  nb_occ;
} Mot;

typedef struct {
    Mot *tab;
    size_t nb_mots;
    size_t capacite;
} Dico;

void init_dico(Dico *d);
int taille(char chaine[]); // TD5
void copie(char dest[], char origine[]); // TD5
int convertir_str_en_int(char *s); // TP2 (ASCII)
void liberer_dico(Dico *d, InfoMem *infoMem);
int assurer_capacite(Dico *d, InfoMem *infoMem); // TD6

int  comparer_mots(char *a, char *b); // TP2 (ASCII) et TD 5 (Chaines de caractères)
int  ajouter_mot(Dico *d, char *mot_lu, InfoMem *infoMem); // TD 6

int  mot_complet(char c);  // TP2 (ASCII)
void compter_fichier(char *nom, Dico *d, InfoMem *infoMem); // TP 7 (Fichiers et chaines de caractères)

void trier_ordre_decroissant(Dico *d);

#endif
