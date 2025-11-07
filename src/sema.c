#include "sema.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Sem_Infos *Semaphores_Infos_Init(int size){

    Sem_Infos *Infos = (Sem_Infos *)malloc(size * sizeof(Sem_Infos));
    if(Infos == NULL){
        perror("Error in Semaphores_Infos_Init,(malloc)");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < size; i++){

        /* If we are in the first Semaphore we Initialize with the START_SEMA_NAME */
        if(i == 0)
            strcpy(Infos[i].Name,START_SEM_NAME);
        else
            Semaphores_Next_Name(Infos[i-1].Name,Infos[i].Name);

        Infos[i].Pr_label = NO_LABEL;
        Infos[i].ID = sem_open(Infos[i].Name,O_CREAT|O_EXCL,0600,SEM_INIT_VALUE);
        if(Infos[i].ID == SEM_FAILED){
            perror("Error in Semaphores_Infos_Init,(sem_open)");
            exit(EXIT_FAILURE);
        }
    }

    return Infos;

}

void Semaphores_Infos_Delete(Sem_Infos *Infos, int size){

    for(int i = 0; i < size; i++){
        if(sem_close(Infos[i].ID) == -1){
            perror("Error in Semaphores_Infos_Delete,(sem_close)");
            exit(EXIT_FAILURE);
        }
        if(sem_unlink(Infos[i].Name) == -1){
            perror("Error in Semaphores_Infos_Delete,(sem_unlink)");
            exit(EXIT_FAILURE);            
        }
    }

    free(Infos);

    return;
}

void Semaphores_Next_Name(const char *last_name, char *new_name){
    strcpy(new_name,last_name);
    (*new_name)++;
}
