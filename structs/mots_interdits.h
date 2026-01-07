#ifndef MotInterdit_H
#define MotInterdit_H

#include <stdio.h>
#include "alloc.h"
#include "outils.h"

typedef struct Mi_cellule {
    char *mot;
    struct Mi_cellule *suivant;
} MiCellule;

typedef struct {
    MiCellule *tete;
} MotInterdit;

void init_mots_interdits(MotInterdit *mi);
void liberer_mots_interdits(MotInterdit *mi, InfoMem *infoMem);
void lire_mots_interdits(MotInterdit *mi, InfoMem *infoMem);
int  est_mot_interdit(char *mot, MotInterdit *mi);

#endif
