#include "server_fun.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int Server_Execute_Instractions(Node *node, int Max_Procces_Exist, int *Procces_Exist, Sem_Infos *Infos, int size){

    int potition = -1; //the potition in that i will put the Procces
    char arrive_string[8], label_string[4];
    
    /* in case we must EXEC */
    sprintf(arrive_string,"%d",node->inst.Time_Snapshot);
    sprintf(label_string,"%d",node->inst.Label);

    /* Base in SPAWN-TERMINATE */
    switch(node->inst.State){

        case SPAWN:
            /* In case i have more than M procces that exists */
            if(*Procces_Exist == Max_Procces_Exist){
                printf("\nOut of resources, Max Processes =%d\n",Max_Procces_Exist);
                return 1;
            }

            /* Go and SPWAN the client */
            (*Procces_Exist)++;

            potition = Find_Empty_Potition(Infos,size);
            Infos[potition].Pr_label = node->inst.Label; //Save the informations

            pid_t p_id = fork();
            if(p_id == -1){
                perror("Fork faild");
                return 1; //so i can close and unlick the semaphores
            }

            if(p_id == 0){
                execlp(CLIENT_PATH,CLIENT_PATH,Infos[potition].Name,SERVER_SEM_NAME,arrive_string,label_string,NULL);
                perror("EXECLP FAILD");
                exit(EXIT_FAILURE);
            }

            break;

        case TERMINATE:
            
            /* In case you give me to terminate a procces that has never SPAWN at all */
            if((potition = Find_Pr_Label(Infos,Max_Procces_Exist,node->inst.Label)) == -1){
                printf("\nClient <C%d> was never SPAWN\n\n",node->inst.Label);
                break; //we continue our program normaly
            }

            /* Lets terminate the procces */
            (*Procces_Exist)--;

            Infos[potition].Pr_label = NO_LABEL; //reset the label
            if(sem_post(Infos[potition].ID) == -1){
                perror("Error in Server_Execute_Instractions,(sem_post)");
                exit(EXIT_FAILURE);
            }
            

            if(wait(NULL) == -1){//mazema tou client
                perror("Error in Server_Execute_Instractions,(wait)");
                exit(EXIT_FAILURE);
            }

            break;
        
        case NONE:
            printf("Something went Terible wrong\n");
            break;
    };

    return 0;

}

int Find_Pr_Label(Sem_Infos *Infos, int size, int label){
    int potition = -1;

    for(int i = 0; i < size; i++){
        if(Infos[i].Pr_label == label){
            potition = i;
            break;
        }
    }
    
    return potition;
}

int Find_Empty_Potition(Sem_Infos *Infos, int size){
    int potition = 0;

    for(int i = 0; i < size; i++){
        if(Infos[i].Pr_label == NO_LABEL){
            potition = i;
            break;
        }
    }

    return potition;
}

int Random_Client(Sem_Infos *Infos, int size){
    int temp = 0;

    for(int i = 0; i < size; i++)
        if(Infos[i].Pr_label != NO_LABEL)
            temp++;

    int random = (rand() % temp)+1; //[1,temp]
    int potition = 0, flag = 0;

    for(int i = 0; i < size; i++){
        if(Infos[i].Pr_label != NO_LABEL){
            flag++;
        }
        if(flag == random){
            potition=i;
            break;
        }
    }

    return potition;
}

void End_All_Clients(Sem_Infos *Infos, int size){

    for(int i = 0; i < size; i++){
        if(Infos[i].Pr_label != NO_LABEL){
            sem_post(Infos[i].ID);
            wait(NULL);
        }
    }

}

void Random_Line(char **data_lines, int size, Shared_Block *block){
    int rand_line = rand() % size;
    strcpy(block->buff,data_lines[rand_line]);
}

