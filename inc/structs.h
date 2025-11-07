#ifndef STRUCTS_H
#define STRUCTS_H

/* Apla enas kompsos tropos na exo STATE */
typedef enum{
    NONE = 'A',
    SPAWN = 'S',
    TERMINATE = 'T'
}Process_State; 

/* Xroniko stigmiotipo - Diergasia - entolh */
typedef struct{
    int Time_Snapshot;
    int Label;
    Process_State State; 
}Pr_Instructions;

#endif