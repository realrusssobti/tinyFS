#include <stdio.h>
#include "linkedList.h"
#include <stdlib.h>
list_t * makeList() {
    list_t * list = malloc(sizeof(list_t));
    list->head = NULL;
    list->end = NULL;
    list->size = 0;
    return list;
}
//
//// frees the data
//int cleanListFree(list_t * list) {
//	if (list == NULL) return -1;
//    node_t * curr = list->head, * temp;
//    while (curr != NULL) {
//        temp = curr;
//        curr = curr->next;
//        free(temp);
//        temp = NULL;
//    }
//    list->end = NULL;
//    list->head = NULL;
//    free(list);
//    return 1;
//}
//
//// Doesn't free the data
//int cleanList(list_t * list) {
//    node_t * curr = list->head, * temp;
//    while (curr != NULL) {
//        temp = curr;
//        curr = curr->next;
//        free(temp);
//    }
//    free(list);
//    return 1;
//}
int addtoList(list_t *list, void * data) {
	node_t * newNode = malloc(sizeof(node_t));
	if (!newNode) return -1;

	newNode->data = data; // Assign data to the new node
	newNode->next = NULL; // Ensure the next pointer of the new node is NULL

	if (list->head == NULL) {
		list->head = newNode;
		list->end = newNode;
	} else {
		list->end->next = newNode;
		list->end = newNode;
	}

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
//
//void * removeVal(fileDescriptor FD, list_t *list) {
//	if (list == NULL) return NULL;
//    int i;
//    node_t * node = list->head;
//    void * data;
//    node_t * temp = NULL, *curr = list->head;
//    resNode *n = node->data;
//    for (i = 0; i < list->size; i++) {
//        if (n->fd == FD) break;
//        temp = node;
//        node = node->next;
//        n = node->data;
//
//    }
//    if (temp == NULL) {
//        list->head = curr->next;
//    } else {
//        temp->next = curr->next;
//    }
//    if (curr == list->end) {
//        list->end = temp;
//    }
//    data = curr->data;
//    free(curr);
//    list->size--;
//    return data;
//}

/* Used to find the FD in the file list*/
void * searchListFD(list_t *list, fileDescriptor fd) {

	node_t * node = list->head;
	resNode *n;

	while (node != NULL) {
		n = node->data;
		if (n != NULL) {
			if (n->fd == fd) {
				return n;
			}
		}

		node = node->next;
	}

	return NULL;
}
void printList(list_t *list) {
	node_t * node = list->head;
	resNode *n;
	while (node != NULL) {
		n = node->data;
		printf("FD: %d, Name: %s, Inode: %d, FP: %d\n", n->fd, n->name, n->inodeIndex, n->fp);
		node = node->next;
	}
}

// frees the data
int cleanListFree(list_t * list) {
	if (list == NULL) return -1;
	node_t * curr = list->head, * temp;
	while (curr != NULL) {
		temp = curr;
		curr = curr->next;
//		free(temp->data); // Free the data
		temp = NULL;
//		free(temp);
	}
	list->end = NULL;
	list->head = NULL;
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
	list->end = NULL;
	list->head = NULL;
	free(list);
	return 1;
}

void * removeVal(fileDescriptor FD, list_t *list) {
	if (list == NULL) return NULL;
	int i;
	node_t * node = list->head;
	void * data;
	node_t * temp = NULL, *curr = list->head;
	resNode *n = node->data;
	for (i = 0; i < list->size; i++) {
		if (n->fd == FD) break;
		temp = node;
		node = node->next;
		n = node->data;
	}
	if (temp == NULL) {
		list->head = curr->next;
	} else {
		temp->next = curr->next;
	}
	if (curr == list->end) {
		list->end = temp;
	}
	data = curr->data;
	// Don't free the node here, it might still be used elsewhere
	list->size--;
	return data;
}