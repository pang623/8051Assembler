#ifndef ASSEMBLEFILE_H
#define ASSEMBLEFILE_H

#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"
#include "Tokenizer.h"
#include "CExceptionConfig.h"
#include "CException.h"
#include "Exception.h"

extern DoubleLinkedList *listPtr;
extern int lineNumber;
extern int muteOnNoLabel;

int assembleFile(char *filename);
char *getNextInstructionLineInFile();

#endif // ASSEMBLEFILE_H
