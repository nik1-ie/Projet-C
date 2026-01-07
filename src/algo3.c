#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "outils.h"
#include "algo3.h"

//Fonction renvoyant si une pile est vide ou non.
int estVide(FileAttente *f){
    return (f->debut == NULL);
}

//Fonction initialisant la pile.
void initFileAttente(FileAttente *f){
    f->debut = NULL;
    f->fin = NULL;
    f->capacity = 0;
}

//Fonction retirant le dernier élement de la pile.
void pop(FileAttente *f, InfoMem *info){
    if (f->fin == NULL) return;
    Cell *temp = f->fin;
    // Find the element before fin
    if (f->debut == f->fin) {
        f->debut = NULL;
        f->fin = NULL;
        f->capacity = 0;
    } else {
        Cell *current = f->debut;
        while (current->suivant != temp) {
            current = current->suivant;
        }
        f->fin = current;
        current->suivant = NULL;
        f->capacity--;
    }
    if (temp->mot) myFree(temp->mot, info, taille(temp->mot) + 1);
    myFree(temp, info, sizeof(Cell));
}

//Fonction libérant l'entièreté de la pile.
void liberer_pile(FileAttente *f, InfoMem *info){
    while(f->fin != NULL){
        pop(f, info);
    }
}

// Fonction vérifiant la présence d'un mot dans la pile
Cell *presence(FileAttente *f, char *mot){
    Cell *current = f->debut;
    while (current != NULL){
        if (comparer_mots(current->mot, mot) == 0){
            return current;
        }
        current = current->suivant;
    }
    return NULL;
}

// //Fonction arrangant les elts de pile selon l'occurance (debut = plus grand)
// void orderPile(FileAttente *f){
//     if (f->debut == NULL || f->debut->suivant == NULL) return; // Rien à trier
//     int swapped;
//     do {
//         swapped = 0;
//         Cell *current = f->debut;
//         Cell *prev = NULL;
//         while (current->suivant != NULL) {
//             if (current->occ < current->suivant->occ) {
//                 // Échanger les cellules
//                 Cell *next = current->suivant;
//                 current->suivant = next->suivant;
//                 next->suivant = current;
//                 if (prev == NULL) {
//                     f->debut = next; // Mise à jour du début si nécessaire
//                 } else {
//                     prev->suivant = next;
//                 }
//                 prev = next;
//                 swapped = 1;
//             } else {
//                 prev = current;
//                 current = current->suivant;
//             }
//         }
//         f->fin = current;
//     } while (swapped);
// }

// Fonction OPTIMISÉE : déplace seulement le mot qui a augmenté d'occurrence
void orderPile(FileAttente *f, Cell *changed_cell){
    if (f->debut == NULL || f->debut->suivant == NULL) return;
    
    // Si le mot est déjà au début, pas besoin de le déplacer
    if (f->debut == changed_cell) return;
    
    // Trouve le prédécesseur de changed_cell
    Cell *prev = f->debut;
    while (prev->suivant != changed_cell) {
        prev = prev->suivant;
    }
    
    // Enlève changed_cell de sa position
    prev->suivant = changed_cell->suivant;
    
    // Insère changed_cell à la bonne position
    Cell *pos = f->debut;
    Cell *prev_pos = NULL;
    
    while (pos != NULL && pos->occ > changed_cell->occ) {
        prev_pos = pos;
        pos = pos->suivant;
    }
    
    changed_cell->suivant = pos;
    if (prev_pos == NULL) {
        f->debut = changed_cell;
    } else {
        prev_pos->suivant = changed_cell;
    }
    
    // Met à jour f->fin
    Cell *temp = f->debut;
    while (temp->suivant != NULL) {
        temp = temp->suivant;
    }
    f->fin = temp;
}


// Fonction ajoutant un mot ou une occurence à la pile 
int stack(FileAttente *f, char *motAjout, InfoMem *info, int min_longueur, MotInterdit *mi, int *nb_mots_total){
    if (taille(motAjout) < min_longueur) {
        return 1;
    }

    if (mi != NULL && est_mot_interdit(motAjout, mi)){
        return 1;
    }

    if (nb_mots_total != NULL) {
        (*nb_mots_total)++;
    }
    
    // Création d'une cellule pour le mot à ajouter
    Cell *nv = (Cell *)myMalloc(sizeof(Cell), info);
    if (nv==NULL) {
        fprintf(stderr, "Erreur d'allocation");
        return 0;
    }
    int len = taille(motAjout) + 1;
    nv->mot = (char *)myMalloc(len, info);
    if (nv->mot == NULL) return 0;
    copie(nv->mot, motAjout);
    nv->suivant = NULL;
    if (nb_mots_total != NULL) (*nb_mots_total)++;

    // si pile vide
    if (estVide(f)){
        nv->occ = 1;
        f->debut = nv;
        f->fin = nv;
        f->capacity++;
    } else {
        Cell *temp = presence(f, motAjout);
        if (temp != NULL){ 
            (*temp).occ++;
            myFree(nv->mot, info, len);
            myFree(nv, info, sizeof(Cell));
            orderPile(f, temp);
        } else {
            nv->occ = 1;
            f->fin->suivant = nv;
            f->fin = nv;
            f->capacity++;
        }
    }
    return 1;
}

// Fonction traitant tout les mots d'un fichier texte.
int traite_tout(FileAttente *f, InfoMem *info, char *file, int min_longueur, MotInterdit *mi, int *nb_mots_total, int minlongueur){
    char c;
    char mot_entier[256];
    int i = 0;
    FILE *file_input = fopen(file, "r");
    if (!file_input) {
        fprintf(stderr, "Le fichier %s ne peut pas être lu.", file);
    }else {
        while ( (c = fgetc(file_input)) != EOF){
            if (mot_complet((char)c)) {
                if (i < 255) mot_entier[i++] = (char)c;
            } else {
                if (i > 0) {
                    mot_entier[i] = '\0';
                    if (taille(mot_entier) >= minlongueur) {
                        stack(f, mot_entier, info, min_longueur, mi, nb_mots_total);
                    }
                    i = 0;
                }
            }
        }
        if (i > 0) {
            mot_entier[i] = '\0';
            if (taille(mot_entier) >= minlongueur) {
                stack(f, mot_entier, info, min_longueur, mi, nb_mots_total);
            }
        }
        fclose(file_input);
    }
    return 1;
}
