#ifndef LABELINFO_H
#define LABELINFO_H

typedef struct LabelInfo LabelInfo;

struct LabelInfo {
  char *name;
  int indexNo;
  int lineNo;
};

LabelInfo *createLabelInfo(LabelInfo *info);
void freeLabelInfo(void *info);
char *createLabelName(char *labelToCreate);

#endif // LABELINFO_H
