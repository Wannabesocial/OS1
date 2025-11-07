#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>

#include "shmemmo.h"

#define CLIENT_SEM_NAME_POS 1
#define SERVER_SEM_NAME_POS 2
#define CLIENT_TIME_POS 3
#define LABEL_POS 4

int main(int argc, char **argv){

    int arrival = atoi(argv[CLIENT_TIME_POS]), label = atoi(argv[LABEL_POS]), end = 0, times_read = 0;
    char buff[SIZE];

    sem_t *server_sem = sem_open(argv[SERVER_SEM_NAME_POS],0); //open server semaphores
    if(server_sem == SEM_FAILED){
        perror("Error in Client,(sem_open)");
        exit(EXIT_FAILURE);        
    }

    sem_t *client_sem = sem_open(argv[CLIENT_SEM_NAME_POS],0); //open my semaphores
    if(client_sem == SEM_FAILED){
        perror("Error in Client,(sem_open)");
        exit(EXIT_FAILURE);   
    }

    Shared_Block *block = Attach_Block(false);

    printf("\nSPAWN (C%d,%d)\n\n",label,arrival);
    fflush(stdout);

    while(true){
        if(sem_wait(client_sem) == -1){ //go block w8 the server to wake you up
            perror("Error in Client,(sem_wait)");
            exit(EXIT_FAILURE);
        }
        
        if(block->Terminate == true){
            end = block->End_Time; //save the terminate time
            break;
        }
        
        times_read++;

        //go and read the buffer from share memmory and print it
        strcpy(buff,block->buff);
        printf("(C%d)%s",label,buff);

        if(sem_post(server_sem) == -1){//waik up the server.I will not go here in case i terminate
            perror("Error in Client,(sem_post)");
            exit(EXIT_FAILURE);
        }
    }

    printf("\nTERMIANTE (C%d,alive time: %d,read times: %d)\n\n",label,end-arrival,times_read);
    fflush(stdout);

    if(sem_close(server_sem) == -1){
        perror("Error in Client,(sem_close)");
        exit(EXIT_FAILURE);
    }
    if(sem_close(client_sem) == -1){
        perror("Error in Client,(sem_close)");
        exit(EXIT_FAILURE); 
    }
    
    Detach_Block(block);

    return 0;
}