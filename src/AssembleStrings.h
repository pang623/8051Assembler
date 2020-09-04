#ifndef ASSEMBLESTRINGS_H
#define ASSEMBLESTRINGS_H

#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"

extern int lineNumber;
extern int muteOnNoLabel;
extern DoubleLinkedList *listPtr;

int assembleStrings();
char *getNextInstructionLineInString();

#endif // ASSEMBLESTRINGS_H
