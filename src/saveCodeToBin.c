#include "saveCodeToBin.h"

void saveCodeToBin(char *filename, uint8_t *codeMemory, int length) {
  FILE *fptr;
  
  if((fptr = fopen(filename, "wb")) == NULL) {
    printf("Error opening file!\n");
    exit(1);
  }
  fwrite(codeMemory, sizeof(uint8_t), length, fptr);
  fclose(fptr);
}