#ifndef ASSEMBLEFILE_H
#define ASSEMBLEFILE_H

#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "MemAlloc.h"
#include "SaveCodeToBin.h"
#include "ExceptionThrowing.h"

void assembleInFileAndWriteToOutFile(char *inFile, char *outFile);
int assembleFile(char *filename);
char *getNextInstructionLineInFile();

#endif // ASSEMBLEFILE_H