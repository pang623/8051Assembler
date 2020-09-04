#include "AssembleFile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *fileHandler;

int assembleInFileAndWriteToOutFile(char *inFile, char *outFile) {
  int totalBytes = assembleFile(inFile);
  saveCodeToBin(outFile, codeMemory, totalBytes);
  return totalBytes;
}

int assembleFile(char *filename) {
  int totalBytes;
  listPtr = doubleLinkedListCreateList();
  if((fileHandler = fopen(filename, "r")) == NULL) {
    printf("Error opening file!\n");
    exit(-1);
  }

  lineNumber = 0;
  muteOnNoLabel = 1;
  totalBytes = assembleInstructions(getNextInstructionLineInFile);

  rewind(fileHandler);

  lineNumber = 0;
  muteOnNoLabel = 0;
  totalBytes = assembleInstructions(getNextInstructionLineInFile);

  fclose(fileHandler);
  doubleLinkedListFreeList(listPtr, freeLabelInfo);
  return totalBytes;
}

char *getNextInstructionLineInFile() {
  char buffer[1024];
  char *line;

  if(fgets(buffer, 1024, fileHandler) != NULL) {
    line = strdup(buffer);
    lineNumber += 1;
    return line;
  }else
    return NULL;
}