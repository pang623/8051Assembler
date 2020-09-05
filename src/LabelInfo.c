#include "LabelInfo.h"
#include "MemAlloc.h"
#include <stdio.h>
#include <string.h>

LabelInfo *createLabelInfo(LabelInfo *info) {
  LabelInfo *infoPtr = memAlloc(sizeof(LabelInfo));
  infoPtr->name      = info->name;
  infoPtr->indexNo   = info->indexNo;
  infoPtr->lineNo    = info->lineNo;
  return infoPtr;
}

void freeLabelInfo(void *info) {
  if((LabelInfo *)info) {
    if(((LabelInfo *)info)->name)
      freeLabelName(((LabelInfo *)info)->name);
    memFree((LabelInfo *)info);
  }
}

char *createLabelName(char *labelToCreate) {
  int len = strlen(labelToCreate);
  char *labelName = memAlloc(len + 1);
  strncpy(labelName, labelToCreate, len);
  labelName[len] = '\0';
  return labelName;
}

void freeLabelName(void *labelName) {
  if((char *)labelName)
    memFree(labelName);
}