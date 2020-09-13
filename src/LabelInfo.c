#include "LabelInfo.h"
#include "MemoryAlloc.h"
#include <stdio.h>
#include <string.h>

LabelInfo *createLabelInfo(LabelInfo *info) {
  LabelInfo *infoPtr = memoryAlloc(sizeof(LabelInfo));
  infoPtr->name      = info->name;
  infoPtr->indexNo   = info->indexNo;
  infoPtr->lineNo    = info->lineNo;
  return infoPtr;
}

void freeLabelInfo(void *info) {
  if((LabelInfo *)info) {
    if(((LabelInfo *)info)->name)
      freeLabelName(((LabelInfo *)info)->name);
    memoryFree((LabelInfo *)info);
  }
}

char *createLabelName(char *labelToCreate) {
  int len = strlen(labelToCreate);
  char *labelName = memoryAlloc(len + 1);
  strncpy(labelName, labelToCreate, len);
  labelName[len] = '\0';
  return labelName;
}

void freeLabelName(void *labelName) {
  if((char *)labelName)
    memoryFree(labelName);
}