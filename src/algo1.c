#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "outils.h"
#include "algo1.h"


void init_dico(Dico *d) {
    d->tab = NULL;
    d->nb_mots = 0;
    d->capacite = 0;
}

void liberer_dico(Dico *d, InfoMem *infoMem) {
    for (int i = 0; i < d->nb_mots; i++) {
        if (d->tab[i].mot != NULL) {
            myFree(d->tab[i].mot, infoMem, taille(d->tab[i].mot) + 1);
        }
    }
    myFree(d->tab, infoMem, d->capacite * sizeof(Mot));
}


int assurer_capacite(Dico *d, InfoMem *infoMem) { // TD6
    if (d->nb_mots >= d->capacite) {
        int nouvelle_cap = (d->capacite == 0) ? 16 : d->capacite * 2;
        int old_size = d->capacite * sizeof(Mot);
        int new_size = nouvelle_cap * sizeof(Mot);
        Mot *nv = myRealloc(d->tab, new_size, infoMem, old_size);
        if (nv == NULL) return 0;
        d->tab = nv;
        d->capacite = nouvelle_cap;
        // printf("assurer_capacite : nb_mots=%d capacite=%d -> nouvelle_cap=%d (old_size=%d, new_size=%d)\n", 
        //     d->nb_mots, d->capacite, nouvelle_cap, old_size, new_size);
    }
    return 1;
}

int ajouter_mot_dico(Dico *d, char *mot_lu, InfoMem *infoMem, int min_longueur, MotInterdit *mi) {
    if (taille(mot_lu) < min_longueur) {
        return 1;
    }

    if (mi != NULL && est_mot_interdit(mot_lu, mi)){
        return 1;
    }

    for (int i = 0; i < d->nb_mots; i++) {
        if (comparer_mots(mot_lu, d->tab[i].mot) == 0) {
            d->tab[i].nb_occ++;
            return 1;
        }
    }

    if (!assurer_capacite(d, infoMem)) return 0;

    int len = taille(mot_lu) + 1;
    d->tab[d->nb_mots].mot = myMalloc(len, infoMem);
    if (d->tab[d->nb_mots].mot == NULL) return 0;

    copie(d->tab[d->nb_mots].mot, (char *)mot_lu);
    d->tab[d->nb_mots].nb_occ = 1;
    d->nb_mots++;
    // printf("ajouter_mot_dico : mot='%s' len=%d\n", mot_lu, len);

    return 1;
}


void compter_fichier_dico(char *nom, Dico *d, InfoMem *infoMem, int min_longueur, MotInterdit *mi) {
    FILE *f = fopen(nom, "r");
    if (f) {
        char mot_courant[256];
        int i = 0;
        int c;

        while ((c = fgetc(f)) != EOF) {
            if (mot_complet((char)c)) {
                if (i < 255) mot_courant[i++] = (char)c;
            } else {
                if (i > 0) {
                    mot_courant[i] = '\0';
                    ajouter_mot_dico(d, mot_courant, infoMem, min_longueur, mi);
                    i = 0;
                }
            }
        }
        if (i > 0) {
            mot_courant[i] = '\0';
            ajouter_mot_dico(d, mot_courant, infoMem, min_longueur, mi);
        }

        fclose(f);
    } else {
        printf("Le fichier ne peut pas être lu.\n");
    }
}



void trier_dico_decroissant(Dico *d) { // TD 7
    for (int i = 0; i + 1 < d->nb_mots; i++) {
        int indice_max = i;

        for (int j = i + 1; j < d->nb_mots; j++) {
            if (d->tab[j].nb_occ > d->tab[indice_max].nb_occ) {
                indice_max = j;
            }
        }

        if (indice_max != i) {
            Mot tmp      = d->tab[i];
            d->tab[i]       = d->tab[indice_max];
            d->tab[indice_max] = tmp;
        }
    }
}

