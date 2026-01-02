#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

typedef struct {
    size_t cumul_alloc;
    size_t cumul_desalloc;
    size_t max_alloc;
} InfoMem;

void *myMalloc(size_t s, InfoMem *infoMem);
void  myFree(void *ptr, InfoMem *infoMem, size_t old_size);
void *myRealloc(void *ptr, size_t new_size, InfoMem *infoMem, size_t old_size);

#endif
