#include "LabelInfo.h"
#include "MemAlloc.h"
#include <stdio.h>

LabelInfo *createLabelInfo(LabelInfo *info) {
  LabelInfo *infoPtr = memAlloc(sizeof(LabelInfo));
  infoPtr->name      = info->name;
  infoPtr->indexNo   = info->indexNo;
  infoPtr->lineNo    = info->lineNo;
  return infoPtr;
}

void freeLabelInfo(void *info) {
  if((LabelInfo *)info)
    memFree((LabelInfo *)info);
}