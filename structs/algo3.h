#ifndef ALGO_PILES_H
#define ALGO_PILES_H

#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "outils.h"
#include "mots_interdits.h"

typedef struct cellule4 {
    char *mot;
    int occ;
    struct cellule4 *suivant;
} Cell;
typedef Cell *List;

typedef struct {
    List debut;
    List fin;
    int capacity;
} FileAttente;

void initFileAttente(FileAttente *f);
int traite_tout(FileAttente *f, InfoMem *info, char *file, int min_longueur, MotInterdit *mi, int *nb_mots_total);
void liberer_pile(FileAttente *f, InfoMem *info);
#endif