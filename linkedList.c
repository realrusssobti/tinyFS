#include <stdio.h>
#include "linkedList.h"

list_t * makeList() {
    list_t * list = malloc(sizeof(list_t));
    list->head = NULL;
    list->size = 0;
    return list;
}

// frees the data
int cleanListFree(list_t * list) {
    node_t * curr = list->head, * temp;
    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp->data);
        free(temp);
    }
    free(list);
    return 1; 
}

// Doesn't free the data 
int cleanList(list_t * list) {
    node_t * curr = list->head, * temp;
    while (curr != NULL) {
        temp = curr;
        curr = curr->next;
        free(temp);
    }
    free(list);
    return 1; 
}

int addtoList(list_t *list, void * data) {
    node_t * newNode = malloc(sizeof(node_t));
    if (!newNode) return -1;

    list->end->next = newNode;
    list->end = newNode;
    list->size++;
    return 1;
}
/* IDK if we need this
int addtoStart(list_t *list, void * data) {
    node_t * newNode = malloc(sizeof(node_t));
    if (!newNode) return -1;
    newNode->data = data;
    newNode->next = list->head;
    return 1;
}
*/

void * removeVal(int idx, list_t *list) {
    int i;
    void * data;
    node_t * temp, *curr = list->head;
    if (list->size - 1 < idx) {
        return -1;
    }

    if (i == 0) {
        temp = curr->next;
        free(curr);
        list->head = temp;
        return 1;
    }

    for (i = 0; i < idx; i++) {
        temp = curr;
        curr = curr->next;
    }

    temp->next = curr->next;
    data = curr->data;
    free(curr);
    list->size--;
    return data;
}

/* Used to find the FD in the file list*/
char * searchList(list_t list, int fd) {

}