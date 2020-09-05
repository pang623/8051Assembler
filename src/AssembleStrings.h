#ifndef ASSEMBLESTRINGS_H
#define ASSEMBLESTRINGS_H

#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"

extern DoubleLinkedList *listPtr;
extern int lineNumber;
extern int muteOnNoLabel;

int assembleStrings();
char *getNextInstructionLineInString();

#endif // ASSEMBLESTRINGS_H
