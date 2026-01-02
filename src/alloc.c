#include <stdio.h>
#include <stdlib.h>
#include "alloc.h"

void *myMalloc(size_t s, InfoMem *infoMem) {
    void *ptr = malloc(s);
    if ( ptr == NULL){
        fprintf(stderr, "Erreur d'allocation memoire \n");
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
        fprintf(stderr, "Erreur d'allocation memoire \n");
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
            // Realloc sans deplacement
if (new_size > old_size) {
                infoMem->cumul_alloc += (new_size - old_size);
            } else {
                infoMem->cumul_desalloc += (old_size - new_size);
            }
        } else {
            // Realloc avec deplacement
            infoMem->cumul_desalloc += old_size;
            infoMem->cumul_alloc += new_size;
        }
        ptr = new_ptr;
                if (infoMem->cumul_alloc - infoMem->cumul_desalloc > infoMem->max_alloc)
            infoMem->max_alloc = infoMem->cumul_alloc - infoMem->cumul_desalloc;
        if ( ptr == NULL){
            fprintf(stderr, "Erreur d'allocation memoire \n");
            return NULL;
        }
    }
    return ptr;
}