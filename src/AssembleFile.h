#ifndef ASSEMBLEFILE_H
#define ASSEMBLEFILE_H

#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"
#include "SaveCodeToBin.h"

extern uint8_t codeMemory[];
extern DoubleLinkedList *listPtr;
extern int lineNumber;
extern int muteOnNoLabel;

int assembleInFileAndWriteToOutFile(char *inFile, char *outFile);
int assembleFile(char *filename);
char *getNextInstructionLineInFile();

#endif // ASSEMBLEFILE_H
