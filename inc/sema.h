#ifndef SEMA_H
#define SEMA_H

#include <semaphore.h>
#include <stdbool.h>
#include <fcntl.h>

#define START_SEM_NAME "A" /* Starting name of my client Semaphores */
#define SERVER_SEM_NAME "SERVER"
#define NO_LABEL 0 //lables starts from 1
#define SEM_NAME_LEN 4
#define SEM_INIT_VALUE 0

/* All the necessary information do semaphores work properly*/
typedef struct{
    int Pr_label; /* See if the semaphores with name (Name) is used or not from some process based on label */
    char Name[SEM_NAME_LEN]; /* Semaphores name */
    sem_t *ID; /* Semaphores ID */
}Sem_Infos;

/* Initialize the Array with the Sema_Infos with MAX_PROCCES size ,return Pointer to array:success,EXIT_FAILURE:fail */
Sem_Infos *Semaphores_Infos_Init(int size);

/* Close the Semaphores, Unlick the Semaphores and dealocate the memmory for the Semaphores Infos  */
void Semaphores_Infos_Delete(Sem_Infos *Infos, int size);

/* Set the name of the next Semaphore based on the name of the previous Semaphore (1st name:A,2cond name:B,etc) 
!CARE YOU SHOULD PASS A ARRAY OF 2 BYTES AT LEAST FOR ARGUMENT2! */
void Semaphores_Next_Name(const char *last_name, char *new_name);

#endif