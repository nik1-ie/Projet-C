// --------- ALGORITHME DE tabE NON TRIEE

#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "algo1.h"
#include "algo2.h"





void AddIntab(Dico *dico, char *mot, InfoMem *infoMem ){
    // Search for existing Mot
    for (int i = 0; i < dico->nb_mots; i++) {
        if (comparer_mots(mot, dico->tab[i].mot) == 0) {
            dico->tab[i].nb_occ++;
            return;
        }
    }
    // Add new Mot if not found
    if (dico->nb_mots < 100) {
        assurer_capacite(dico, infoMem);
        int len = taille(mot) + 1;
        dico->tab[dico->nb_mots].mot = (char *)myMalloc(len, infoMem);
        copie(dico->tab[dico->nb_mots].mot, mot);
        dico->tab[dico->nb_mots].nb_occ = 1;
        dico->nb_mots++;
    }
}

int log_Mots(Dico *d, char *fichier, InfoMem *infoMem ){
    char c;
    char mot_entier[256];
    int i = 0;
    FILE *file = fopen(fichier, "r");
    if (!file) {
        fprintf(stderr, "Le fichier %s ne peut pas être lu.", fichier);
    }else {
        while ( (c = fgetc(file)) != EOF){
            if (mot_complet((char)c)) {
                if (i < 255) mot_entier[i++] = (char)c;
            } else {
                if (i > 0) {
                    mot_entier[i] = '\0';
                    AddIntab(d, mot_entier, infoMem);
                    i = 0;
                }
            }
        }
        if (i > 0) {
            mot_entier[i] = '\0';
            AddIntab(d, mot_entier, infoMem);
        }
        fclose(file);
    }
    return 1;
}