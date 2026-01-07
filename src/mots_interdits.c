#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "outils.h"
#include "mots_interdits.h"

void init_mots_interdits(MotInterdit *mi) {
    mi->tete = NULL;
}

void liberer_mots_interdits(MotInterdit *mi, InfoMem *infoMem) {
    MiCellule *c = mi->tete;
    while (c != NULL) {
        MiCellule *suiv = c->suivant;
        if (c->mot != NULL) {
            myFree(c->mot, infoMem, taille(c->mot) + 1);
        }
        myFree(c, infoMem, sizeof(MiCellule));
        c = suiv;
    }
    mi->tete = NULL;
}

void lire_mots_interdits(MotInterdit *mi, InfoMem *infoMem) {
    char mot_courant[256];

    printf("Entrez les mots interdits un par un (tapez /fin pour terminer) :\n");

    while (1) {
        printf("> ");
        if (scanf("%255s", mot_courant) != 1) {
            break;
        }
        if (comparer_mots(mot_courant, "/fin") == 0) {
            break;
        }

        MiCellule *nv = myMalloc(sizeof(MiCellule), infoMem);
        if (nv == NULL) {
            printf("Erreur d'allocation pour stopword\n");
            break;
        }

        int len = taille(mot_courant) + 1;
        nv->mot = myMalloc(len, infoMem);
        if (nv->mot == NULL) {
            myFree(nv, infoMem, sizeof(MiCellule));
            printf("Erreur d'allocation pour stopword\n");
            break;
        }

        copie(nv->mot, mot_courant);
        nv->suivant = mi->tete;
        mi->tete = nv;
    }
}

int est_mot_interdit(char *mot, MotInterdit *mi) {
    MiCellule *c = mi->tete;
    while (c != NULL) {
        if (comparer_mots(mot, c->mot) == 0) {
            return 1;
        }
        c = c->suivant;
    }
    return 0;
}
