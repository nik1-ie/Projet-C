#include <stdio.h>
#include <stdlib.h>

typedef struct {
    size_t cumul_alloc;    // champ obligatoire : cumul de l’espace mémoire alloué
    size_t cumul_desalloc; // champ obligatoire : cumul de l’espace mémoire désalloué
    size_t max_alloc;      // pic d'allocation (diff max des deux cumuls)

    // d’autres champs qui sembleraient utiles
} InfoMem;

void *myMalloc(size_t s, InfoMem *infoMem) {
    void *ptr = malloc(s);
    if ( ptr == NULL){
        fprintf(stderr, "Erreur d'allocation mémoire \n");
        return NULL;
    }
    infoMem->cumul_alloc += s;
    if (infoMem->cumul_alloc - infoMem->cumul_desalloc > infoMem->max_alloc) {
        infoMem->max_alloc = infoMem->cumul_alloc - infoMem->cumul_desalloc;
    }
    return ptr;
}

void myFree(void *ptr, InfoMem *infoMem, size_t old_size) {
    if (ptr == NULL){
        fprintf(stderr, "Erreur d'allocation mémoire \n");
        return;
    }

    infoMem -> cumul_desalloc += old_size;
    free(ptr);
}

void *myRealloc(void *ptr, size_t new_size, InfoMem *infoMem, size_t old_size) {
    if (ptr == NULL) {
        void *new_ptr = realloc(ptr, new_size);
        if (new_ptr == NULL) {
            fprintf(stderr, "Erreur d'allocation memoire\n");
            return NULL;
        }
        infoMem->cumul_alloc += new_size;
        if (infoMem->cumul_alloc - infoMem->cumul_desalloc > infoMem->max_alloc)
            infoMem->max_alloc = infoMem->cumul_alloc - infoMem->cumul_desalloc;
        return new_ptr;
    } 
    void *new_ptr = realloc(ptr, new_size);

    if (new_ptr == NULL){
        fprintf(stderr, "Erreur d'allocation memoire\n");
        return NULL;
    } else{
        if (new_ptr == ptr) {
            // Realloc sans déplacement
if (new_size > old_size) {
                infoMem->cumul_alloc += (new_size - old_size);
            } else {
                infoMem->cumul_desalloc += (old_size - new_size);
            }
        } else {
            // Realloc avec déplacement
            infoMem->cumul_desalloc += old_size;
            infoMem->cumul_alloc += new_size;
        }
        ptr = new_ptr;
                if (infoMem->cumul_alloc - infoMem->cumul_desalloc > infoMem->max_alloc)
            infoMem->max_alloc = infoMem->cumul_alloc - infoMem->cumul_desalloc;
        if ( ptr == NULL){
            fprintf(stderr, "Erreur d'allocation mémoire \n");
            return NULL;
        }
    }
    return ptr;
}
