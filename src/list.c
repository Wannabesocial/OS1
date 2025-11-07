#include "list.h"
#include <stdio.h>
#include <stdlib.h>

/* Here all the source code for the List */

void List_Init(List_MetaData *MetaData){
    MetaData->Min_Time_Snapshot = 0;
    MetaData->Max_Time_Snapshot = 0;
    MetaData->start = NULL;
    MetaData->end = NULL;    
    MetaData->it = NULL;
}

bool List_Is_Empty(List_MetaData MetaData){
    return (MetaData.start == NULL) ? true : false;
}


void Node_Set(Node *node, int time, int label, Process_State state){
    /* Just go and set the data in the Node */
    node->inst.Time_Snapshot = time;
    node->inst.Label = label;
    node->inst.State = state;
    node->next = NULL;
}

void List_Push(List_MetaData *MetaData, int time, int label, Process_State state){

    Node *new_node = (Node *)malloc(sizeof(Node));
    if(new_node == NULL){
        perror("Error in List_Push,(malloc)");
        exit(EXIT_FAILURE);
    }

    Node_Set(new_node,time,label,state);

    /* If list is empty (start) and (end) "look" in the same node */
    if(List_Is_Empty(*MetaData)){
        MetaData->Min_Time_Snapshot = time;
        MetaData->start = new_node;
        MetaData->end = new_node;
        MetaData->it = MetaData->start; /* Itrerator 'see' the start */
        return;
    }

    /* We push in the back of the list */
    MetaData->end->next = new_node;
    MetaData->end = new_node;
}

void List_It_Init(List_MetaData *MTD){
    MTD->it = MTD->start;
}

Node *List_Node_It(List_MetaData *MTD){

    /* There is nothing left in the list */
    if(MTD->it == NULL)
        return NULL;
    
    Node *node = MTD->it;
    MTD->it = MTD->it->next;

    return node;

}

void Node_Delete(List_MetaData *MTD){
    /* Delete the first Node of the list */
    Node *temp_node = MTD->start;
    MTD->start = MTD->start->next;
    free(temp_node);
}

void List_Delete(List_MetaData *MetaData){
    while(List_Is_Empty(*MetaData) == false){
        Node_Delete(MetaData);
    }
}

void List_Print(List_MetaData MetaData){
    printf("METADATA|min_time=%d,max_time=%d\n",MetaData.Min_Time_Snapshot,MetaData.Max_Time_Snapshot);
    
    Node *temp_node = MetaData.start;
    while(temp_node != NULL){
        printf("(%d,%d,%c)\n",temp_node->inst.Time_Snapshot,temp_node->inst.Label,temp_node->inst.State);
        temp_node = temp_node->next;
    }
}
