#ifndef ALGO_LCHAINE_PILES_H
#define ALGO_LCHAINE_PILES_H

#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "algo2.h"

typedef struct cellule {
    char *mot;
    int occ;
    struct cellule *suivant;
} Cellule, *Liste;

typedef struct {
    Liste debut ;
    Liste fin ;
} FileAttente ;

// void initFileAttente(FileAttente *f);
int log_mots4(FileAttente *file, char *fichier, InfoMem *infoMem);
void fileToDico(FileAttente *file, Dico *dico, InfoMem *infoMem);
#endif