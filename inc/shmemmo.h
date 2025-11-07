#ifndef SHMEMMO_H
#define SHMEMMO_H

#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdbool.h>

#define FILE_NAME "server.c"
#define PROJECT_ID 'S'
#define SIZE 1024

typedef struct{
    char buff[SIZE];
    bool Terminate; //fist checked from (client) so he know he must terminate
    int End_Time;   
}Shared_Block;

/* Go and make the shared memmory fragment and return the ID
if (server) call this function creat the fragmen else just open it */
int Shared_Block_ID(bool Create);

/* Attach the Shared Block in the procces */
Shared_Block *Attach_Block(bool Create);

/* Just go and detach the shared memmory from the procces */
void Detach_Block(Shared_Block *shared_block);

/* Go and destroy the shared memmory */
void Destroy_Block();

#endif