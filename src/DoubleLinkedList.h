#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include "ListItem.h"

typedef void (*FreeFunction)(void *);
typedef struct DoubleLinkedList DoubleLinkedList;
struct DoubleLinkedList{
  ListItem *head;
  ListItem *tail;
  int count;
};

int doubleLinkedListAddItemToHead(DoubleLinkedList *list, ListItem *item);
int doubleLinkedListAddItemToTail(DoubleLinkedList *list, ListItem *item);
ListItem* doubleLinkedListRemoveItemFromHead(DoubleLinkedList *ListPtr);
ListItem* doubleLinkedListRemoveItemFromTail(DoubleLinkedList *ListPtr);
ListItem* doubleLinkedListCreateListItem(void *data);
void doubleLinkedListFreeListItem(ListItem* itemToFree);
DoubleLinkedList *doubleLinkedListCreateList();
void doubleLinkedListFreeList(DoubleLinkedList *ListPtr, FreeFunction freeFunc);

#endif // DOUBLELINKEDLIST_H
