#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "outils.h"
#include "algo2.h"


void init_liste(Liste *L) {
    L->tete = NULL;
}

void liberer_liste(Liste *L, InfoMem *infoMem) {
    Cellule *courant = L->tete;
    while (courant != NULL) {
        Cellule *suiv = courant->suivant;
        if (courant->mot != NULL) {
            myFree(courant->mot, infoMem, taille(courant->mot) + 1);
        }
        myFree(courant, infoMem, sizeof(Cellule));
        courant = suiv;
    }
    L->tete = NULL;
}

int compter_cellules(Liste *L) {
    int n = 0;
    Cellule *c = L->tete;
    while (c != NULL) {
        n++;
        c = c->suivant;
    }
    return n;
}

Cellule *recherche(Liste *L, char *mot) { // TD 7
    Cellule *courant = L->tete;
    while (courant != NULL) {
        if (comparer_mots(mot, courant->mot) == 0) {
            return courant;
        }
        courant = courant->suivant;
    }
    return NULL;
}

int ajouter_mot_liste(Liste *L, char *mot_lu, InfoMem *infoMem, int min_longueur, MotInterdit *mi) {
    if (taille(mot_lu) < min_longueur) {
        return 1;
    }

    if (mi != NULL && est_mot_interdit(mot_lu, mi)){
        return 1;
    }

    Cellule *c = recherche(L, mot_lu);
    if (c != NULL) {
        c->nb_occ++;
        return 1;
    }

    Cellule *nv = myMalloc(sizeof(Cellule), infoMem);
    if (nv == NULL) return 0;

    int len = taille(mot_lu) + 1;
    nv->mot = myMalloc(len, infoMem);
    if (nv->mot == NULL) {
        myFree(nv, infoMem, sizeof(Cellule));
        return 0;
    }
    copie(nv->mot, (char *)mot_lu);

    nv->nb_occ = 1;
    nv->suivant = L->tete;
    L->tete = nv;

    return 1;
}



void compter_fichier_liste(char *nom, Liste *L, InfoMem *infoMem, int min_longueur, MotInterdit *mi) {
    FILE *f = fopen(nom, "r");
    if (!f) {
        printf("Erreur ouverture fichier %s\n", nom);
        exit(EXIT_FAILURE);
    }

    char mot_courant[256];
    int i = 0;
    int c;

    while ((c = fgetc(f)) != EOF) {
        if (mot_complet((char)c)) {
            if (i < 255) mot_courant[i++] = (char)c;
        } else {
            if (i > 0) {
                mot_courant[i] = '\0';
                ajouter_mot_liste(L, mot_courant, infoMem, min_longueur, mi);
                i = 0;
            }
        }
    }
    if (i > 0) {
        mot_courant[i] = '\0';
        ajouter_mot_liste(L, mot_courant, infoMem, min_longueur, mi);
    }

    fclose(f);
}



void trier_liste_decroissante(Liste *L) {
    Cellule *i = L->tete;
    while (i != NULL && i->suivant != NULL) {
        Cellule *max = i;
        Cellule *j = i->suivant;
        while (j != NULL) {
            if (j->nb_occ > max->nb_occ) {
                max = j;
            }
            j = j->suivant;
        }
        if (max != i) {
            char *tmp_mot = i->mot;
            int   tmp_occ = i->nb_occ;

            i->mot = max->mot;
            i->nb_occ = max->nb_occ;

            max->mot = tmp_mot;
            max->nb_occ = tmp_occ;
        }
        i = i->suivant;
    }
}