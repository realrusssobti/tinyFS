#ifndef LINKEDLIST
#define LINKEDLIST

#include "libTinyFS.h"
typedef struct Node {
   void *data;
   struct Node *next;
} node_t;

typedef struct List {
   node_t *head;
   node_t * end;
   int size;
} list_t;

list_t * makeList();
int cleanList(list_t * list);
int addtoList(list_t *list, void * data);
void * removeVal(int idx, list_t *list);
int cleanListFree(list_t * list);
void * searchListFD(list_t *list, fileDescriptor fd);
#endif