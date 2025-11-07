#ifndef SERVER_FUN_H
#define SERVER_FUN_H

#include "list.h"
#include "sema.h"
#include "shmemmo.h"

#define CLIENT_PATH "client"
#define MAX_PROCCES_POTITION 1
#define DATA_FILE_PATH_POS 2
#define CONFIG_CHOISE_POS 3
#define CONFIGE_BUFF_SIZE 1024

/* 
Here we execute the instraction that we have in server. 
Here is posible posible to have already M existed procceses,so if i must SPAWN enather one i should terminate the program
0:succes,1:fail
*/
int Server_Execute_Instractions(Node *node, int Max_Procces_Exist, int *Procces_Exist, Sem_Infos *Infos, int size);

/* 
Go and check if is even exist a client with a specific label 
return potition:succes,-1:fail*/
int Find_Pr_Label(Sem_Infos *Infos, int size, int label);

/* Find empty space so i can give it in the client i must spawn */
int Find_Empty_Potition(Sem_Infos *Infos, int size);

/* Return the random potition from the array of SEM_infos this client read the data */
int Random_Client(Sem_Infos *Infos, int size);

/* Go and terminate procceses that has not terminate */
void End_All_Clients(Sem_Infos *Infos, int size);

/* Go and save a random line from data to the shared memmory */
void Random_Line(char **data_lines, int size, Shared_Block *block);

#endif