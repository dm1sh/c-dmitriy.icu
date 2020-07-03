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

/**
 * @brief Check if array contains string
 * 
 * @param arr 
 * @param length 
 * @param str 
 * @return int 
 */
int check_if_contains(char **arr, size_t length, char *str)
{
    for (int i = 0; i < length; i++)
        if (strcmp(arr[i], str) == 0)
            return i;

    return 0;
}