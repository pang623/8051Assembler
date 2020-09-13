#include <stdio.h>
#include <stdlib.h>
#include "MemoryAlloc.h"

void *memoryAlloc(int size) {
  return malloc(size);
}

void memoryFree(void *memToFree) {
  free(memToFree);
}