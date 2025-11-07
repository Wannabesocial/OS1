#ifndef READ_FUNS_H
#define READ_FUNS_H

#define READ_BUFFER_SIZE 8
#define DATA_BUFFER_SIZE 1024

#define SMALL_CONFIG "information/config_3_100.txt"
#define MID_CONFIG "information/config_3_1000.txt"
#define BIG_CONFIG "information/config_10_10000.txt"

#include "list.h"

/* Go and read from the config file ,EXIT_FAILURE:error-fail */
void Read_Config_File(List_MetaData *MetaData, const char *path_name);

/* Take a String and seperate the 'C' for the start with the Integer who follows */
int Seperate_Integer_Label(char *Procces);

/* Go and find the total number of rows from the txt then go alocate memmory
for a array of strings.Then go and save every line that you read in the array and return it */
char **Read_Data_File(const char *file_path, int *size);

/* Alocate the memmory from the array with the valuable lines */
void Data_Lines_Delete(char **Data_lines, int size);

/* Go take the path that the user gives you for the confige */
void Take_Config(char *config_file, int choise);

#endif