#include <stdio.h>
#include <stdlib.h>
#include "MemAlloc.h"

void *memAlloc(int size) {
  return malloc(size);
}

void memFree(void *memToFree) {
  free(memToFree);
}