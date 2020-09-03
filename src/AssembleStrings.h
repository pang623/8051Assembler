#ifndef ASSEMBLESTRINGS_H
#define ASSEMBLESTRINGS_H

#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "MemAlloc.h"
#include "LabelInfo.h"
#include "ExceptionThrowing.h"

int assembleStrings();
char *getNextInstructionLineInString();

#endif // ASSEMBLESTRINGS_H
