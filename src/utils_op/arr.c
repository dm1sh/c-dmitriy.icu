#include "../../include/utils_op/arr.h"
#include <stdio.h>

/**
 * @brief Insert string to the end of array of strings
 * 
 * @param arr 
 * @param length 
 * @param value 
 */
void insert_to_arr(char ***arr, size_t length, char *value)
{
    *arr = realloc(*arr, (length + 1) * sizeof(char *));

    (*arr)[length] = malloc(strlen(value) * sizeof(char));
    strcpy((*arr)[length], value);
}

/**
 * @brief Free memory allocated for array of strings
 * 
 * @param arr 
 * @param length 
 */
void free_arr(char **arr, int length)
{
    for (int i = 0; i < length; i++)
        free(arr[i]);

    free(arr);
}