#include "AssembleStrings.h"
#include "String.h"
#include <stdlib.h>

int lineIndex = 0;

char **instructionLines;

int assembleStrings() {
  int totalBytes;
  listPtr = doubleLinkedListCreateList();
  
  for(int pass = 1; pass != 3; pass++) {
    lineNumber = 0;
    lineIndex = 0;
    muteOnNoLabel = !(pass - 1);
    totalBytes = assembleInstructions(getNextInstructionLineInString);
  }
  doubleLinkedListFreeList(listPtr, freeLabelInfo);
  return totalBytes;
}

char *getNextInstructionLineInString() {
  int len;
  char *line;
  if(instructionLines[lineIndex] != NULL) {
    len = strlen(instructionLines[lineIndex]);
    line = malloc(len+1);
    strncpy(line, instructionLines[lineIndex++], len);
    line[len] = '\0';
    lineNumber += 1;
    return line;
  }else
    return NULL;
}