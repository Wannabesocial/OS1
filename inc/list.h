#ifndef LIST_H
#define LIST_H

#include "structs.h"
#include <stdbool.h>

/* Here we have all the template for (list) functions */

typedef struct node{
    Pr_Instructions inst;
    struct node *next;
}Node;

/* Keep all the important infos i need */
typedef struct metadata{
    int Min_Time_Snapshot; /* The time my first command will execute (from config) */
    int Max_Time_Snapshot; /* The time i must end the Program (from config) */
    Node *it; /* So i can iterate the link list */
    Node *start; /* For fast delete O(1) */
    Node *end; /* For fast insert O(1) */
}List_MetaData;

/* Initialize the MetaData */
void List_Init(List_MetaData *MetaData);

/* Check if link list is empty */
bool List_Is_Empty(List_MetaData MetaData);

/* Push data in the list (insert end) */
void List_Push(List_MetaData *MetaData, int time, int label, Process_State state);

/* Initialize the Iterator */
void List_It_Init(List_MetaData *MTD);

/* Return the Data in the potition of the Iterator,and move the iterator to the next node if exist */
Node *List_Node_It(List_MetaData *MTD);

/* Dealocate all the memmory from the list */
void List_Delete(List_MetaData *MetaData);

/* Print all the data of the list */
void List_Print(List_MetaData MetaData);

#endif