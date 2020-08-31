#include <stdio.h>
#include <stdlib.h>
#include "MemAlloc.h"

void *memAlloc(int size) {
  printf("memAlloc is called\n");
  return malloc(size);
}

void memFree(void *memToFree) {
  printf("memFree is called\n");
  free(memToFree);
}