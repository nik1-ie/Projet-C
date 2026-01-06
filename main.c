#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "alloc.h"
#include "outils.h"
#include "options.h"
#include "algo1.h"
#include "algo2.h"
#include "algo3.h"

int main(int argc, char *argv[]) {
    time_t graine = time(NULL);
    time_t debut, fin;
    debut = time(NULL);
    InfoMem info = (InfoMem){0, 0, 0};
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
        } else if (comparer_mots(argv[i], "-k") == 0) {
            if (i + 1 >= argc) {
                printf("Option -k attend un entier\n");
            }
            opt.min_longueur = convertir_str_en_int(argv[++i]);
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
            i++;
        }
    }

    if (opt.afficher_aide) {
        printf("./projet [-n int] [-k int] [-a algo1|algo2] [--help] "
               "[-s fichierdesortie] [-l fichierdeperf] fichiers...\n");
        return EXIT_SUCCESS;
    }

    if (i >= argc) {
        printf("Aucun fichier de donnees fourni.\n");
        return EXIT_FAILURE;
    }

    if (opt.nom_algo == NULL || comparer_mots(opt.nom_algo, "algo1") == 0) {
        Dico D;
        init_dico(&D);

        for (; i < argc; i++) {
            compter_fichier_dico(argv[i], &D, &info, opt.min_longueur);
        }

        trier_dico_decroissant(&D);
        FILE *out = stdout;
        if (opt.fichier_sortie != NULL) {
            out = fopen(opt.fichier_sortie, "w");
            if (!out) {
                printf("Erreur lors de l'ouverture du fichier de sortie\n");
                liberer_dico(&D, &info);
                return EXIT_FAILURE;
            }
        }

        int limite;
        if (opt.nb_mots_a_afficher < 0) {
            limite = (int)D.nb_mots;
        } else {
            limite = opt.nb_mots_a_afficher;
        }
        if (limite > (int)D.nb_mots) {
            limite = (int)D.nb_mots;
        }

        for (int k = 0; k < limite; k++) {
            fprintf(out, "%s %d\n", D.tab[k].mot, D.tab[k].nb_occ);
        }

        if (out != stdout) {
            fclose(out);
        }

        fin = time(NULL) - debut;
        if (opt.fichier_perf != NULL) {
            FILE *pf = fopen(opt.fichier_perf, "w");
            if (!pf) {
                printf("Erreur lors de l'ouverture du fichier de performances\n");
            } else {
                fprintf(pf, "cumul_alloc=%zu\n", info.cumul_alloc);
                fprintf(pf, "cumul_desalloc=%zu\n", info.cumul_desalloc);
                fprintf(pf, "max_alloc=%zu\n", info.max_alloc);
                fprintf(pf, "temps d'exécution=%d\n", fin);
                fclose(pf);
            }
        }

        liberer_dico(&D, &info);

    } else if (comparer_mots(opt.nom_algo, "algo2") == 0) {
        Liste L;
        init_liste(&L);

        for (; i < argc; i++) {
            compter_fichier_liste(argv[i], &L, &info, opt.min_longueur);
        }

        trier_liste_decroissante(&L);

        FILE *out = stdout;
        if (opt.fichier_sortie != NULL) {
            out = fopen(opt.fichier_sortie, "w");
            if (!out) {
                printf("Erreur lors de l'ouverture du fichier de sortie\n");
                liberer_liste(&L, &info);
                return EXIT_FAILURE;
            }
        }

        int nb_mots = compter_cellules(&L);

        int limite;
        if (opt.nb_mots_a_afficher < 0) {
            limite = nb_mots;
        } else {
            limite = opt.nb_mots_a_afficher;
        }
        if (limite > nb_mots) {
            limite = nb_mots;
        }

        Cellule *courant = L.tete;
        int k = 0;
        while (courant != NULL && k < limite) {
            fprintf(out, "%s %d\n", courant->mot, courant->nb_occ);
            courant = courant->suivant;
            k++;
        }

        if (out != stdout) {
            fclose(out);
        }

        fin = time(NULL) - debut;
        
        if (opt.fichier_perf != NULL) {
            FILE *pf = fopen(opt.fichier_perf, "w");
            if (!pf) {
                printf("Erreur lors de l'ouverture du fichier de performances\n");
            } else {
                fprintf(pf, "cumul_alloc=%zu\n", info.cumul_alloc);
                fprintf(pf, "cumul_desalloc=%zu\n", info.cumul_desalloc);
                fprintf(pf, "max_alloc=%zu\n", info.max_alloc);
                fprintf(pf, "temps d'exécution=%d\n", fin);
                fclose(pf);
            }
        }

        liberer_liste(&L, &info);

    } else if (comparer_mots(opt.nom_algo, "algo3") == 0) {
        FileAttente fileatt;
        initFileAttente(&fileatt);
        for (; i < argc; i++) {
            traite_tout(&fileatt, &info, argv[i]);
        }
        
        FILE *out = stdout;
        if (opt.fichier_sortie != NULL) {
            out = fopen(opt.fichier_sortie, "w");
            if (!out) {
                printf("Erreur lors de l'ouverture du fichier de sortie\n");
                liberer_pile(&fileatt, &info);
                return EXIT_FAILURE;
            }
        }

        int nb_mots = fileatt.capacity;

        int limite;
        if (opt.nb_mots_a_afficher < 0) {
            limite = nb_mots;
        } else {
            limite = opt.nb_mots_a_afficher;
        }

        if (limite > nb_mots) {
            limite = nb_mots;
        }

        Cell *resultats = (Cell *)myMalloc(sizeof(Cell) * limite, &info);
        for (int j = 0; j < limite; j++) {
            fprintf(out, "%s %d\n", resultats[j].mot, resultats[j].occ);

        }

        if (out != stdout) {
            fclose(out);
        }

        fin = time(NULL) - debut;

        if (opt.fichier_perf != NULL) {
            FILE *pf = fopen(opt.fichier_perf, "w");
            if (!pf) {
                printf("Erreur lors de l'ouverture du fichier de performances\n");
            } else {
                fprintf(pf, "cumul_alloc=%zu\n", info.cumul_alloc);
                fprintf(pf, "cumul_desalloc=%zu\n", info.cumul_desalloc);
                fprintf(pf, "max_alloc=%zu\n", info.max_alloc);
                fprintf(pf, "temps d'exécution=%d\n", fin);
                fclose(pf);
            }
        }
        liberer_pile(&fileatt, &info);


    } else {
        printf("Algorithme inconnu : %s\n", opt.nom_algo);
        printf("Algorithmes disponibles :\n algo1, algo2, algo3\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
