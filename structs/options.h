#ifndef OPTIONS_H
#define OPTIONS_H

#include <stddef.h>

typedef struct {
    int   nb_mots_a_afficher; // -n
    char *nom_algo;           // -a
    char *fichier_sortie;     // -s
    char *fichier_perf;       // -l
    int   afficher_aide;      // --help
    int   min_longueur;       // -k : longueur minimale
} Options;


void init_options(Options *opt);

#endif