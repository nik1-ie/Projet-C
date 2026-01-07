#ifndef ALGO_LISTE_H
#define ALGO_LISTE_H

#include <stdio.h>
#include "alloc.h"
#include "outils.h"
#include "mots_interdits.h"

typedef struct cellule {
    char *mot;
    int   nb_occ;
    struct cellule *suivant;
} Cellule;

typedef struct {
    Cellule *tete;
} Liste;

void init_liste(Liste *L);
void liberer_liste(Liste *L, InfoMem *infoMem);
Cellule *recherche(Liste *L, char *mot);
int ajouter_mot_liste(Liste *L, char *mot_lu, InfoMem *infoMem, int min_longueur, MotInterdit *mi);
void compter_fichier_liste(char *nom, Liste *L, InfoMem *infoMem, int min_longueur, MotInterdit *mi);

void trier_liste_decroissante(Liste *L);

int  compter_cellules(Liste *L);

#endif
