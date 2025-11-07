#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "read_funs.h"
#include "sema.h"
#include "server_fun.h"
#include "shmemmo.h"

#include <unistd.h>
#include <sys/wait.h>
#include <time.h>


int main(int argv, char **argc){

    if(argv != 4) //in case you give less or more than the needed arguments
        return 1;

    srand(time(NULL));

    int Max_Procces_Exist = atoi(argc[MAX_PROCCES_POTITION]);
    int data_size = 0, Procces_Exist = 0, ser_ex = 0, rand_c = -1;
    char **data_lines = Read_Data_File(argc[DATA_FILE_PATH_POS],&data_size);//diabasma grammon apo to arxeio

    char config_file[CONFIGE_BUFF_SIZE];
    Take_Config(config_file,atoi(argc[CONFIG_CHOISE_POS]));//go and take the confige path base on users reference

    List_MetaData MTD;
    List_Init(&MTD);
    
    Read_Config_File(&MTD,config_file);

    /* Creat the share memmory */
    Shared_Block *block = Attach_Block(true);

    sem_t *server_sem = sem_open(SERVER_SEM_NAME,O_CREAT|O_EXCL,0600,SEM_INIT_VALUE);
    if(server_sem == SEM_FAILED){
        perror("Error in Server,(sem_open)");
        exit(EXIT_FAILURE); 
    }

    Sem_Infos *Infos = Semaphores_Infos_Init(Max_Procces_Exist);
   
    bool exist_command = true; //i have a current comand to run and wait the correct time to execute it
    List_It_Init(&MTD);
    Node *list_node = List_Node_It(&MTD);

    int x = 0; //this is in case we must terminate the program couse of resourcec
    for(int i = MTD.Min_Time_Snapshot; i <= MTD.Max_Time_Snapshot; i++){

        /* does not exist a commant to run and i have commands left */
        if(exist_command == false && list_node->next != NULL){
            exist_command = true;
            list_node = List_Node_It(&MTD);
        }

        /* its time to run the command (i=command's time snapshot) */
        if(exist_command && i == list_node->inst.Time_Snapshot){

            /* Set the Terminate is true in case we must Terminate a procces and mark the End_Time */
            block->Terminate = true;
            block->End_Time = i;
            ser_ex = Server_Execute_Instractions(list_node,Max_Procces_Exist,&Procces_Exist,Infos,Max_Procces_Exist);
            /* Reset the Terminate in shared memmory so we will not terminate a client by mistake */
            block->Terminate = false;

            /* In case i must Execute SPAWN when i already have M existed procceses */
            if(ser_ex == 1){
                x = i;
                break;
            }

            exist_command = false; //ready to pull the next command if exist
        }

        /* Check is exist at least one client so i can give him a job */
        if(Procces_Exist == 0){
            continue;
        }

        Random_Line(data_lines,data_size,block); //go and save the random line in shared memmory
        rand_c = Random_Client(Infos,Max_Procces_Exist); //give to a randmon client the commant to read
                
        //here we go and read in share memmory one random thing!
        if(sem_post(Infos[rand_c].ID) == -1){//awake the client
            perror("Error in Server,(sem_post)");
            exit(EXIT_FAILURE);            
        }

        if(sem_wait(server_sem) == -1){//server go to sleep and w8 the client to wake him up after reading the share memmory
            perror("Error in Server,(sem_wait)");
            exit(EXIT_FAILURE);            
        }
    }
    
    /* Terminate procceses that has not terminate */
    block->Terminate = true;
    block->End_Time = (x == 0) ? MTD.Max_Time_Snapshot : x;
    End_All_Clients(Infos,Max_Procces_Exist);

    List_Delete(&MTD);
    Semaphores_Infos_Delete(Infos,Max_Procces_Exist);

    if(sem_close(server_sem) == -1){
        perror("Error in Server,(sem_close)");
        exit(EXIT_FAILURE);
    }

    if(sem_unlink(SERVER_SEM_NAME) == -1){
        perror("Error in Server,(sem_unlink)");
        exit(EXIT_FAILURE); 
    }

    Detach_Block(block);
    Destroy_Block();

    Data_Lines_Delete(data_lines,data_size);

    return 0;
}