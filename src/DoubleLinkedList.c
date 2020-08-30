#include "DoubleLinkedList.h"
#include "MemAlloc.h"
#include <stdio.h>


//Add ListItem to the head of double linked list and return the new count of items in the list.
//@1stparam Pointer to double linked list
//@2ndparam Pointer to list item
//@retval Number of items in the list after the addition
int doubleLinkedListAddItemToHead(DoubleLinkedList *list, ListItem *item) {
  if(item == NULL)
    return list->count;

  item->next = list->head;
  item->prev = NULL;

  if(list->head == NULL)
    list->tail = item;
  else
    (list->head)->prev = item;

  list->head = item;
  list->count += 1;

  return list->count;
}


//Add ListItem to the tail of double linked list and return the new count of items in the list.
//@1stparam Pointer to double linked list
//@2ndparam Pointer to list item
//@retval Number of items in the list after the addition
int doubleLinkedListAddItemToTail(DoubleLinkedList *list, ListItem *item) {
  if(item == NULL)
    return list->count;

  item->prev = list->tail;
  item->next = NULL;

  if(list->head == NULL)
    list->head = item;
  else
    (list->tail)->next = item;

  list->tail = item;
  list->count += 1;

  return list->count;
}


//Remove ListItem from the head of double linked list and return the item removed from the list.
//@1stparam Pointer to double linked list
//@retval Item that is removed from the head of the list
ListItem* doubleLinkedListRemoveItemFromHead(DoubleLinkedList *ListPtr) {
  ListItem* lastItem;

  if(ListPtr->head == NULL)                     //check is it empty list
    return NULL;                                //no item to be removed, so return lastItem as NULL
  else {
    lastItem = ListPtr->head;                   //lastItem points to the item that is to be removed
    ListPtr->head = lastItem->next;             //item removed from head after this line
    if(ListPtr->head == NULL)                   //after removing the item, if no item left, tail points to NULL
      ListPtr->tail = NULL;
    else
      (ListPtr->head)->prev = NULL;
    lastItem->next = NULL;                      //make sure the next and prev of the removed item points to NULL
    lastItem->prev = NULL;
    ListPtr->count -= 1;                        //decrement count by 1 every time an item is removed
  }
  return lastItem;                              //lastItem which contains the item removed is returned
}


//Remove ListItem from the tail of double linked list and return the item removed from the list.
//@1stparam Pointer to double linked list
//@retval Item that is removed from the tail of the list
ListItem* doubleLinkedListRemoveItemFromTail(DoubleLinkedList *ListPtr) {
  ListItem* LastItem;

  if(ListPtr->head == NULL)
    return NULL;
  else {
    LastItem = ListPtr->tail;
    ListPtr->tail = LastItem->prev;
    if(ListPtr->tail == NULL)
      ListPtr->head = NULL;
    else
      (ListPtr->tail)->next = NULL;
    LastItem->next = NULL;
    LastItem->prev = NULL;
    ListPtr->count -= 1;
  }
  return LastItem;
}

ListItem* doubleLinkedListCreateListItem(void *data) {
  ListItem *item = memAlloc(sizeof(ListItem));
  item->next = NULL;
  item->prev = NULL;
  item->data = data;
  return item;
}

void doubleLinkedListFreeListItem(ListItem* itemToFree) {
  if(itemToFree)
    memFree(itemToFree);
}

DoubleLinkedList *doubleLinkedListCreateList() {
  DoubleLinkedList *list = memAlloc(sizeof(DoubleLinkedList));
  list->head = NULL;
  list->tail = NULL;
  list->count = 0;
  return list;
}

void doubleLinkedListFreeList(DoubleLinkedList *ListPtr, FreeFunction freeFunc) {
  ListItem *item;
  while(ListPtr->head) {
    item = (ListPtr->head)->next;
    freeFunc((ListPtr->head)->data);
    doubleLinkedListFreeListItem(ListPtr->head);
    ListPtr->head = item;
  }
  if(ListPtr)
    memFree(ListPtr);
}