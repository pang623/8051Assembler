#include "AssembleStrings.h"

static int lineIndex;
extern int lineNumber;
extern int muteOnNoLabel;
extern DoubleLinkedList *listPtr;

char *instructionLines[1024];

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
  char *line;

  if(instructionLines[lineIndex] != NULL) {
    lineNumber += 1;
    return line = instructionLines[lineIndex++];
  }else
    return NULL;
}