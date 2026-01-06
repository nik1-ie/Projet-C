#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "algo2.h"
#include "algo4.h"

int estVide(FileAttente *f){
    return (f->debut == NULL);
}

// void initFileAttente(FileAttente *f){
//     f->debut = NULL;
//     f->fin = NULL;
// }

int empiler_mots(char *mot_ajouter, FileAttente *file, InfoMem *infoMem){

    printf("Ajout du mot '%s' à la file d'attente en cours.\n", mot_ajouter);
    Cellule *nouvelle_cellule = (Cellule *)myMalloc(sizeof(Cellule), infoMem);
    if (nouvelle_cellule == NULL) {
        printf("Echec de l'allocation mémoire pour une nouvelle cellule.\n");
        return 0; // Echec de l'allocation
    }

    int len = sizeof(mot_ajouter) + 1;
    nouvelle_cellule->mot = (char *)myMalloc(len, infoMem);
    printf("Cellule vide allouée. \n");
    if (nouvelle_cellule->mot == NULL) {
        printf("Echec de l'allocation mémoire pour le mot dans la cellule.\n");
        return 0;
    }
    printf("Nv cellule et mot alloués avec succès.\n"); // EXEC CHECKPOINT
    // copie(nouvelle_cellule->mot, mot_ajouter);

    Cellule *current = file->debut;
    
    nouvelle_cellule->mot, nouvelle_cellule->suivant, nouvelle_cellule->occ = mot_ajouter, NULL, 1;
    // Si file est vide. mettre directement le mot dedans.
    if (estVide(file)){
        file->debut = nouvelle_cellule;
        file->fin = nouvelle_cellule;
        printf("La file d'attente était vide. Mot ajouté en tant que premier élément.\n");
        return 1;
    } 
    
    while (current->suivant != NULL){ // Parcours de pile si mot présent
        printf("Vérification du mot '%s' dans la file d'attente.\n", current->mot);
        if (comparer_mots(current->mot, mot_ajouter)==0){
            current->occ++;
            myFree(nouvelle_cellule, infoMem, len);
            return 1;
        }
        current = current->suivant;
    }
    printf("Mot '%s' non trouvé dans la file d'attente. Ajout en cours...\n", mot_ajouter);
    
    file->fin->suivant = nouvelle_cellule;
    file->fin = nouvelle_cellule;
    printf("Mot ajouté à la fin de la file d'attente.\n");

    return 1;
}

int log_mots4(FileAttente *file, char *fichier, InfoMem *infoMem){
    printf("Début de la lecture du fichier %s...\n", fichier);
    char c;
    char mot_entier[256];
    int i = 0;
    FILE *file_input = fopen(fichier, "r");
    if (!file_input) {
        fprintf(stderr, "Le fichier %s ne peut pas être lu.", fichier);
    }else {
        while ( (c = fgetc(file_input)) != EOF){
            printf("Caractere lu : %c\n", c);
            if (mot_complet((char)c)) {
                if (i < 255) mot_entier[i++] = (char)c;
            } else {
                if (i > 0) {
                    mot_entier[i] = '\0';
                    empiler_mots(mot_entier, file, infoMem);
                    i = 0;
                }
            }
        }
        if (i > 0) {
            mot_entier[i] = '\0';
            empiler_mots(mot_entier, file, infoMem);
        }
        fclose(file_input);
    }
    return 1;
}

void fileToDico(FileAttente *file, Dico *dico, InfoMem *infoMem){
    Cellule *current = file->debut;
    while (current != NULL){
        // Ajouter chaque mot de la file au dictionnaire
        for (int i = 0; i < dico->nb_mots; i++) {
            if (comparer_mots(current->mot, dico->tab[i].mot) == 0) {
                dico->tab[i].nb_occ += current->occ;
                break;
            }
        }
        // Si mot pas trouvé, l'ajouter
        if (dico->nb_mots < 100) {
            assurer_capacite(dico, infoMem);
            int len = taille(current->mot) + 1;
            dico->tab[dico->nb_mots].mot = (char *)myMalloc(len, infoMem);
            copie(dico->tab[dico->nb_mots].mot, current->mot);
            dico->tab[dico->nb_mots].nb_occ = current->occ;
            dico->nb_mots++;
        }
        current = current->suivant;
    }
}