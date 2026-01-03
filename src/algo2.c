#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "algo2.h"


void init_dico(Dico *d) {
    // initialuse un dictionnaire vide //
    d->tab = NULL;
    d->nb_mots = 0;
    d->capacite = 0;
}


int taille(char chaine[]) { // TD5
    int n = 0;
    while (chaine[n] != '\0') {
        n++;
    }
    return n;
}

void copie(char dest[], char origine[]) { // TD5
    while (*origine != '\0') {
        *dest = *origine;
        dest++;
        origine++;
    }
    *dest = *origine;
}

int convertir_str_en_int(char *s) { // ASCII
    // '0' -> 48    // '5' -> 53
    // '1' -> 49    // '6' -> 54
    // '2' -> 50    // '7' -> 55
    // '3' -> 51    // '8' -> 56
    // '4' -> 52    // '9' -> 57
    int signe = 1;
    int res = 0;
    int i = 0;

    if (s[0] == '-') {
        signe = -1;
        i++;
    } else if (s[0] == '+') {
        i++;
    }

    while (s[i] >= '0' && s[i] <= '9') {
        int chiffre = s[i] - '0';
        res = res * 10 + chiffre;
        i++;
    }
    return signe * res;
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
    }
    return 1;
}

int comparer_mots(char *a, char *b) { // TP2 (ASCII) et TD 5 (Chaines de caractères)
    int i = 0;

    while (a[i] != '\0' && b[i] != '\0') {
        char ca = a[i];
        char cb = b[i];

        if (ca >= 'A' && ca <= 'Z') {
            ca = ca + ('a' - 'A');
        }
        if (cb >= 'A' && cb <= 'Z') {
            cb = cb + ('a' - 'A');
        }

        if (ca != cb) {
            return (int)ca - (int)cb;
        }
        i++;
    }

    return (int)a[i] - (int)b[i];
}

int ajouter_mot(Dico *d, char *mot_lu, InfoMem *infoMem) { // TD 6
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

    return 1;
}

int mot_complet(char c) { // TP2 (ASCII)
    if ((c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        c == '-' || c == '\'') {
        return 1;
    }
    return 0;
}


void compter_fichier(char *nom, Dico *d, InfoMem *infoMem) { // TP 7 (Fichiers et chaines de caractères)
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
                    ajouter_mot(d, mot_courant, infoMem);
                    i = 0;
                }
            }
        }
        if (i > 0) {
            mot_courant[i] = '\0';
            ajouter_mot(d, mot_courant, infoMem);
        }

        fclose(f);
    } else {
        printf("Le fichier ne peut pas être lu.\n");
    }
}


void trier_ordre_decroissant(Dico *d) { // TD 7
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

