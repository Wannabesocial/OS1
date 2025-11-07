#include "shmemmo.h"
#include <stdio.h>
#include <stdlib.h>

int Shared_Block_ID(bool Create){
    key_t key = ftok(FILE_NAME,PROJECT_ID); //try ganarate key
    if(key == -1){ //something goes wrong
        perror("Error in Shared_Block_ID,(ftok)");
        exit(EXIT_FAILURE);
    }

    /* kane the memmory fragment -- an uparxei apla anoikse to */
    int id = (Create) ? shmget(key,sizeof(Shared_Block),IPC_CREAT|IPC_EXCL|0600) : shmget(key,sizeof(Shared_Block),0);
    if(id == -1){
        perror("Error in Shared_Block_ID,(shmget)");
        exit(EXIT_FAILURE);
    }

    return id;
}

Shared_Block *Attach_Block(bool Create){
    int id = Shared_Block_ID(Create); //take the shared memmory id

    Shared_Block *shared_block = NULL;
    /* Attach */
    shared_block = shmat(id,NULL,0);
    if(shared_block == (void *)-1){
        perror("Error in Attach_Block,(shmat)");
        exit(EXIT_FAILURE);
    }

    return shared_block;
}

void Detach_Block(Shared_Block *shared_block){
    int result = shmdt(shared_block);
    if(result == -1){
        perror("Error in Detach_Block,(shmdt)");
    }
}

void Destroy_Block(){
    int id = Shared_Block_ID(false);

    int result = shmctl(id,IPC_RMID,NULL);
    if(result == -1){
        perror("Error in Destroy_Block,(shmctl)");
        exit(EXIT_FAILURE);
    }
}
