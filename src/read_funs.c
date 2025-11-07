#include "read_funs.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

void Read_Config_File(List_MetaData *MetaData, const char *path_name){

    FILE *file = fopen(path_name,"r");
    if(file == NULL){
        perror("Error in Read_Config_File,(fopen)");
        exit(EXIT_FAILURE);
    }

    int time, label;
    char buff[READ_BUFFER_SIZE], Pr_state;
    int read = 0; //track the elements i read (3:all good,2:i must terminate read)

    /* Read the data from the Config.txt file */
    while((read = fscanf(file,"%d %s %c",&time,buff,&Pr_state)) != 2){
        label = Seperate_Integer_Label(buff);
        List_Push(MetaData,time,label,Pr_state);
    }

    /* There was no instractions in config.txt */
    if(List_Is_Empty(*MetaData)){
        printf("No instractions i %s\n",path_name);
        exit(EXIT_FAILURE);
        fclose(file);
    }

    MetaData->Max_Time_Snapshot = time; //i read above only 2 from 3 elements (x_value EXIT)
    fclose(file);
}

int Seperate_Integer_Label(char *Procces){
    Procces++; //skip the 'C' character
    int label = atoi(Procces);
    return label;
}

char **Read_Data_File(const char *file_path, int *size){

    FILE *file = fopen(file_path,"r");
    if(file == NULL){
        perror("Error in Read_Data_File,(fopen)");
        exit(EXIT_FAILURE);
    }

    int lines = 0;
    char buff[DATA_BUFFER_SIZE];

    /* Go and find the number of usefull lines */
    while(fgets(buff,DATA_BUFFER_SIZE,file) != NULL){
        if(strlen(buff) == (size_t)1) /* if "\n" line then i do not want it */
            continue;

        lines++;
    }

    char **Data_lines = (char **)malloc(sizeof(char *) * lines);
    if(Data_lines == NULL){
        perror("Error in Read_Data_File,(malloc)");
        exit(EXIT_FAILURE);
    }

    fseek(file,0,SEEK_SET); //start from the start
    int i = 0; 
    /* Go and find the lines that are usefull and safe them */
    while(fgets(buff,DATA_BUFFER_SIZE,file) != NULL){

        if(strlen(buff) == (size_t)1) /* if "\n" line then i do not want it */
            continue;

        Data_lines[i] = (char *)malloc(sizeof(char) * (int)(strlen(buff)+1));//+1 for '\0'
        if(Data_lines == NULL){
            perror("Error in Read_Data_File,(malloc)");
            exit(EXIT_FAILURE);
        }

        strcpy(Data_lines[i],buff);
        i++;
    }


    fclose(file);
    *size = lines;
    return Data_lines;
}

void Data_Lines_Delete(char **Data_lines, int size){

    for(int i = 0; i < size; i++){
        free(Data_lines[i]);
    }

    free(Data_lines);

}

void Take_Config(char *config_file, int choise){

    switch(choise){
        case 1:
            strcpy(config_file,SMALL_CONFIG);
            break;
        case 2:
            strcpy(config_file,MID_CONFIG);
            break;
        case 3:
            strcpy(config_file,BIG_CONFIG);
            break;
        default:
            printf("Opps this choise does not match!\n");
            exit(EXIT_FAILURE);
    };

}

