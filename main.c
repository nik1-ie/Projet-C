#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"
#include "algo2.h"
#include "options.h"

int main(int argc, char *argv[]) {
    InfoMem info = {0, 0, 0};
    Options opt;
    init_options(&opt);

    int i = 1;

    while (i < argc && argv[i][0] == '-') {
        if (comparer_mots(argv[i], "--help") == 0) {
            opt.afficher_aide = 1;
            i++;
        } else if (comparer_mots(argv[i], "-n") == 0) {
            if (i + 1 >= argc) {
                printf("Option -n attend un entier\n");
            }
            opt.nb_mots_a_afficher = convertir_str_en_int(argv[++i]);
            i++;
        } else if (comparer_mots(argv[i], "-a") == 0) {
            if (i + 1 >= argc) {
                printf("Option -a attend un nom d'algorithme\n");
            }
            opt.nom_algo = argv[++i];
            i++;
        } else if (comparer_mots(argv[i], "-s") == 0) {
            if (i + 1 >= argc) {
                printf("Option -s attend un nom de fichier\n");
            }
            opt.fichier_sortie = argv[++i];
            i++;
        } else if (comparer_mots(argv[i], "-l") == 0) {
            if (i + 1 >= argc) {
                printf("Option -l attend un nom de fichier\n");
            }
            opt.fichier_perf = argv[++i];
            i++;
        } else {
            printf("Option inconnue : %s\n", argv[i]);
        }
    }

    if (opt.afficher_aide) {
        printf("./projet [-n int] [-a algo1|algo2|algo3] [--help] [-s fichierdesortie] [-l fichierdeperf] fichiers...\n");
    }

    if (i >= argc) {
        printf("Aucun fichier de donnees fourni.\n");
    }

    Dico d;
    init_dico(&d);

    if (opt.nom_algo == NULL || comparer_mots(opt.nom_algo, "algo2") == 0) {
        for (; i < argc; i++) {
            compter_fichier(argv[i], &d, &info);
        }
    } else {
        printf("Algorithme inconnu : %s\n", opt.nom_algo);
        printf("Algorithmes disponibles :\n algo2 -> Liste triee");
        liberer_dico(&d, &info);
    }

    trier_ordre_decroissant(&d);

    FILE *out = stdout;
    if (opt.fichier_sortie != NULL) {
        out = fopen(opt.fichier_sortie, "w");
        if (!out) {
            printf("Erreur lors de l'ouverture du fichier de sortie\n");
            liberer_dico(&d, &info);
        }
    }

    int limite;
    if (opt.nb_mots_a_afficher < 0) {
        limite = (int)d.nb_mots;
    } else {
        limite = opt.nb_mots_a_afficher;
    }

    if (limite > (int)d.nb_mots){
        limite = (int)d.nb_mots;
    }

    for (int i = 0; i < limite; i++) {
        fprintf(out, "%s %d\n", d.tab[i].mot, d.tab[i].nb_occ);
    }

    if (out != stdout) {
        fclose(out);
    }

    if (opt.fichier_perf != NULL) {
        FILE *pf = fopen(opt.fichier_perf, "w");
        if (!pf) {
            printf("Erreur lors de l'ouverture du fichier de performances\n");
        } else {
            fprintf(pf, "cumul_alloc=%zu\n", info.cumul_alloc);
            fprintf(pf, "cumul_desalloc=%zu\n", info.cumul_desalloc);
            fprintf(pf, "max_alloc=%zu\n", info.max_alloc);
            fclose(pf);
        }
    }

    liberer_dico(&d, &info);

    return 0;
}
