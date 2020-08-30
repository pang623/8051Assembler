#ifndef LISTITEM_H
#define LISTITEM_H

typedef struct ListItem ListItem;
struct ListItem{
  ListItem *next;
  ListItem *prev;
  void *data;
};

#endif // LISTITEM_H
