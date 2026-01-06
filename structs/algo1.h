#ifndef ALGO1_H
#define ALGO1_H

#include <stddef.h>
#include "alloc.h"
#include "outils.h"

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
void liberer_dico(Dico *d, InfoMem *infoMem);
int assurer_capacite(Dico *d, InfoMem *infoMem); // TD6

int ajouter_mot_dico(Dico *d, char *mot_lu, InfoMem *infoMem, int min_longueur); // TD 6

void compter_fichier_dico(char *nom, Dico *d, InfoMem *infoMem, int min_longueur); // TP 7 (Fichiers et chaines de caractères)

void trier_dico_decroissant(Dico *d);

#endif
