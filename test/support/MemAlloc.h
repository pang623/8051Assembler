#ifndef MEMALLOC_H
#define MEMALLOC_H

#include  <stdlib.h>

void  *memAlloc(int size);
void  memFree(void  *memToFree);

#endif // MEMALLOC_H
