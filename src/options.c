#include "options.h"

void init_options(Options *opt) {
    opt->nb_mots_a_afficher = -1;
    opt->nom_algo = NULL;
    opt->fichier_sortie = NULL;
    opt->fichier_perf = NULL;
    opt->afficher_aide = 0;
}
